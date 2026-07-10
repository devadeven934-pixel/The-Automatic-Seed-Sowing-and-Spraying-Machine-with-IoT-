#pragma once

// =====================================================================================
// Agri-Bot IoT — pin map & configuration
//
// Pin numbers below are NOT dictated by hardware/electrical_connections.json — that file
// only names logical connections (e.g. "Left Motor PWM"), not GPIO numbers. This map is a
// reasonable ESP32-WROOM-32D assignment that avoids strapping pins (0, 2, 12, 15) and
// input-only pins (34-39) for anything that needs to be an output.
//
// ASSUMPTION FLAGGED: the original hardware plan wires only ONE relay (for the water
// pump). It does not specify a control line for the plough linear actuator direction or
// for the spraying solenoid valve. This firmware assumes:
//   - a 2-channel relay (or small H-bridge) drives the linear actuator forward/reverse
//   - a second relay/MOSFET drives the solenoid valve
// If your actual build only has the single pump relay, remove/adjust the actuator and
// solenoid pin usage in ploughing.cpp / spraying.cpp accordingly.
// =====================================================================================

// ---------- Wi-Fi / MQTT ----------
#define WIFI_SSID          "YOUR_WIFI_SSID"
#define WIFI_PASSWORD      "YOUR_WIFI_PASSWORD"

#define MQTT_BROKER        "your-broker-address"   // e.g. broker.hivemq.com, or your own
#define MQTT_PORT          1883
#define MQTT_CLIENT_ID     "agribot-01"
#define MQTT_USER          ""                        // leave blank if not required
#define MQTT_PASSWORD      ""

#define MQTT_TOPIC_TELEMETRY  "agribot/agribot-01/telemetry"
#define MQTT_TOPIC_COMMAND    "agribot/agribot-01/command"
#define MQTT_TOPIC_STATUS     "agribot/agribot-01/status"

#define TELEMETRY_INTERVAL_MS 2000

// ---------- Drive motors (L298N) ----------
#define LEFT_MOTOR_IN1   27
#define LEFT_MOTOR_IN2   26
#define LEFT_MOTOR_PWM   25   // ENA

#define RIGHT_MOTOR_IN3  14
#define RIGHT_MOTOR_IN4  33
#define RIGHT_MOTOR_PWM  13   // ENB

#define PWM_FREQ_HZ      5000
#define PWM_RESOLUTION   8    // 0-255
#define LEFT_PWM_CHANNEL  0
#define RIGHT_PWM_CHANNEL 1

// ---------- Seed sowing servo ----------
#define SERVO_PIN         18
#define SERVO_CLOSED_DEG  0
#define SERVO_OPEN_DEG    90

// ---------- Rotary encoder (wheel distance) ----------
#define ENCODER_CLK_PIN   34   // input-only, fine for digital read
#define ENCODER_DT_PIN    35
#define WHEEL_CIRCUMFERENCE_CM 20.0f
#define ENCODER_PULSES_PER_REV 20

// ---------- Ploughing (linear actuator + limit switch) ----------
// See ASSUMPTION note above.
#define PLOUGH_EXTEND_PIN   21
#define PLOUGH_RETRACT_PIN  22
#define PLOUGH_LIMIT_SWITCH_PIN 32   // active LOW, INPUT_PULLUP

// ---------- Spraying (pump relay + solenoid) ----------
#define PUMP_RELAY_PIN     4     // matches "Relay Control" in electrical_connections.json
#define SOLENOID_PIN       23    // ASSUMPTION — see note above

// ---------- Sensors ----------
#define ULTRASONIC_TRIG_PIN 5
#define ULTRASONIC_ECHO_PIN 19
#define OBSTACLE_STOP_CM    25.0f

#define SOIL_MOISTURE_PIN   36   // ADC1_CH0, input-only
// Calibrate these against your specific sensor: raw ADC value in dry air vs. fully in water.
#define SOIL_ADC_DRY   3000
#define SOIL_ADC_WET   1200

// GPS on UART2
#define GPS_RX_PIN  16   // ESP32 RX2 <- GPS TX
#define GPS_TX_PIN  17   // ESP32 TX2 -> GPS RX
#define GPS_BAUD    9600

// ---------- Seed sowing spacing ----------
#define SEED_SPACING_CM 25.0f
