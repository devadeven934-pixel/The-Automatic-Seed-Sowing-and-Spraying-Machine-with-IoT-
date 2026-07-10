#include "net.h"
#include "config.h"
#include <WiFi.h>
#include <PubSubClient.h>

namespace Net {

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static CommandHandler commandHandler = nullptr;

static unsigned long last_reconnect_attempt_ms = 0;
static const unsigned long RECONNECT_INTERVAL_MS = 5000;

static void mqttCallback(char *topic, byte *payload, unsigned int length) {
    String message;
    message.reserve(length);
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    if (commandHandler != nullptr) {
        commandHandler(message);
    }
}

static void connectWifi() {
    if (WiFi.status() == WL_CONNECTED) return;

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("[WiFi] Connecting");
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
        delay(300);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("[WiFi] Connected, IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("[WiFi] Connection failed, will retry.");
    }
}

static bool connectMqtt() {
    if (WiFi.status() != WL_CONNECTED) return false;

    bool ok;
    if (strlen(MQTT_USER) > 0) {
        ok = mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD);
    } else {
        ok = mqttClient.connect(MQTT_CLIENT_ID);
    }

    if (ok) {
        Serial.println("[MQTT] Connected");
        mqttClient.subscribe(MQTT_TOPIC_COMMAND);
        mqttClient.publish(MQTT_TOPIC_STATUS, "online", true);
    } else {
        Serial.print("[MQTT] Connect failed, rc=");
        Serial.println(mqttClient.state());
    }
    return ok;
}

void begin(CommandHandler handler) {
    commandHandler = handler;
    connectWifi();

    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    connectMqtt();
}

void update() {
    if (WiFi.status() != WL_CONNECTED) {
        connectWifi();
        return;
    }

    if (!mqttClient.connected()) {
        unsigned long now = millis();
        if (now - last_reconnect_attempt_ms > RECONNECT_INTERVAL_MS) {
            last_reconnect_attempt_ms = now;
            connectMqtt();
        }
        return;
    }

    mqttClient.loop();
}

void publishTelemetry(const String &json) {
    if (mqttClient.connected()) {
        mqttClient.publish(MQTT_TOPIC_TELEMETRY, json.c_str());
    }
}

bool isConnected() {
    return mqttClient.connected();
}

} // namespace Net
