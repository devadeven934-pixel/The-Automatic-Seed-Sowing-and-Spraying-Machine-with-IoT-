// =====================================================================================
// Agri-Bot IoT — main firmware
//
// Modes (set remotely via MQTT command topic, JSON: {"mode": "sow"}):
//   idle    - everything stopped
//   manual  - drive controlled directly via {"mode":"manual","left":N,"right":N}
//   plough  - lower the tilling blade, drive forward, raise on stop
//   sow     - drive forward, drop seeds at fixed spacing (see SEED_SPACING_CM)
//   spray   - drive forward, pump + solenoid on
//   auto    - drive forward; stop automatically on obstacle; combine plough+sow+spray
//             is left as a project-specific extension (see TODO below)
//
// Safety: in every driving mode, the ultrasonic sensor is checked each loop and the
// robot stops immediately if an obstacle is closer than OBSTACLE_STOP_CM.
// =====================================================================================

#include <Arduino.h>
#include <ArduinoJson.h>

#include "config.h"
#include "drive.h"
#include "sowing.h"
#include "ploughing.h"
#include "spraying.h"
#include "sensors.h"
#include "net.h"

enum Mode { MODE_IDLE, MODE_MANUAL, MODE_PLOUGH, MODE_SOW, MODE_SPRAY, MODE_AUTO };
static Mode currentMode = MODE_IDLE;

static const int DEFAULT_DRIVE_SPEED = 180;
static unsigned long lastTelemetryMs = 0;

static const char *modeToString(Mode m) {
    switch (m) {
        case MODE_IDLE:   return "idle";
        case MODE_MANUAL: return "manual";
        case MODE_PLOUGH: return "plough";
        case MODE_SOW:    return "sow";
        case MODE_SPRAY:  return "spray";
        case MODE_AUTO:   return "auto";
    }
    return "unknown";
}

static void stopEverything() {
    Drive::stop();
    Sowing::stop();
    Ploughing::stop();
    Spraying::stop();
}

static void enterMode(Mode newMode) {
    stopEverything();
    currentMode = newMode;

    switch (newMode) {
        case MODE_PLOUGH:
            Ploughing::lower();
            break;
        case MODE_SOW:
            Sowing::start();
            break;
        case MODE_SPRAY:
            Spraying::start();
            break;
        default:
            break;
    }

    Serial.print("[Mode] -> ");
    Serial.println(modeToString(newMode));
}

// Handles JSON commands arriving over MQTT, e.g.:
//   {"mode": "sow"}
//   {"mode": "manual", "left": 150, "right": 150}
//   {"mode": "idle"}
static void handleCommand(const String &payload) {
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err) {
        Serial.print("[Cmd] JSON parse error: ");
        Serial.println(err.c_str());
        return;
    }

    const char *modeStr = doc["mode"] | "";

    if (strcmp(modeStr, "idle") == 0) {
        enterMode(MODE_IDLE);
    } else if (strcmp(modeStr, "manual") == 0) {
        enterMode(MODE_MANUAL);
        int left = doc["left"] | 0;
        int right = doc["right"] | 0;
        Drive::setSpeeds(left, right);
    } else if (strcmp(modeStr, "plough") == 0) {
        enterMode(MODE_PLOUGH);
    } else if (strcmp(modeStr, "sow") == 0) {
        enterMode(MODE_SOW);
    } else if (strcmp(modeStr, "spray") == 0) {
        enterMode(MODE_SPRAY);
    } else if (strcmp(modeStr, "auto") == 0) {
        enterMode(MODE_AUTO);
    } else if (strlen(modeStr) > 0) {
        Serial.print("[Cmd] Unknown mode: ");
        Serial.println(modeStr);
    }
}

static void publishTelemetry() {
    StaticJsonDocument<384> doc;
    doc["mode"] = modeToString(currentMode);
    doc["soil_moisture_pct"] = Sensors::readSoilMoisturePercent();
    doc["obstacle_cm"] = Sensors::readUltrasonicCm();
    doc["distance_travelled_cm"] = Sowing::distanceTravelledCm();
    doc["gps_fix"] = Sensors::hasGpsFix();
    if (Sensors::hasGpsFix()) {
        doc["lat"] = Sensors::gpsLatitude();
        doc["lng"] = Sensors::gpsLongitude();
        doc["satellites"] = Sensors::gpsSatellites();
    }
    doc["plough_limit_reached"] = Ploughing::isLimitReached();
    doc["spraying"] = Spraying::isActive();
    doc["mqtt_connected"] = Net::isConnected();

    String json;
    serializeJson(doc, json);
    Net::publishTelemetry(json);
}

// Non-blocking obstacle check used by driving modes.
static bool obstacleAhead() {
    float d = Sensors::readUltrasonicCm();
    return (d > 0 && d < OBSTACLE_STOP_CM);
}

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n[Agri-Bot IoT] Booting...");

    Drive::begin();
    Sowing::begin();
    Ploughing::begin();
    Spraying::begin();
    Sensors::begin();
    Net::begin(handleCommand);

    stopEverything();
    Serial.println("[Agri-Bot IoT] Ready.");
}

void loop() {
    Net::update();
    Sensors::update();
    Ploughing::update();
    Sowing::update();

    // Obstacle safety cutoff applies to every driving mode.
    bool blocked = obstacleAhead();

    switch (currentMode) {
        case MODE_IDLE:
        case MODE_MANUAL:
            // Manual mode: speeds are set directly by incoming commands; nothing to do here.
            break;

        case MODE_PLOUGH:
            if (blocked) {
                Drive::stop();
            } else {
                Drive::forward(DEFAULT_DRIVE_SPEED);
            }
            break;

        case MODE_SOW:
            if (blocked) {
                Drive::stop();
            } else {
                Drive::forward(DEFAULT_DRIVE_SPEED);
            }
            break;

        case MODE_SPRAY:
            if (blocked) {
                Drive::stop();
            } else {
                Drive::forward(DEFAULT_DRIVE_SPEED);
            }
            break;

        case MODE_AUTO:
            // TODO: project-specific extension — e.g. run plough -> sow -> spray in
            // sequence across a field, or combine them based on GPS waypoints.
            // Currently behaves like sow mode with obstacle avoidance as a placeholder.
            if (blocked) {
                Drive::stop();
            } else {
                Drive::forward(DEFAULT_DRIVE_SPEED);
                if (!Sowing::isActive()) Sowing::start();
            }
            break;
    }

    unsigned long now = millis();
    if (now - lastTelemetryMs >= TELEMETRY_INTERVAL_MS) {
        lastTelemetryMs = now;
        publishTelemetry();
    }
}
