#pragma once
#include <Arduino.h>

namespace Net {

// Command callback: invoked whenever a JSON command arrives on MQTT_TOPIC_COMMAND.
// payload is the raw JSON string.
typedef void (*CommandHandler)(const String &payload);

void begin(CommandHandler handler);

// Call every loop() — maintains Wi-Fi/MQTT connections and processes incoming messages.
void update();

void publishTelemetry(const String &json);
bool isConnected();

} // namespace Net
