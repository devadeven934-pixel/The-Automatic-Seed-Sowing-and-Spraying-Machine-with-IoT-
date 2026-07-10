#pragma once
#include <Arduino.h>

namespace Sensors {

void begin();

// Call every loop() to feed the GPS parser from the UART buffer.
void update();

// Ultrasonic obstacle distance in cm (returns -1 on timeout/no echo).
float readUltrasonicCm();

// Soil moisture, 0 (dry) - 100 (saturated), based on ADC reading.
int readSoilMoisturePercent();

// GPS — valid only once a fix has been acquired (hasGpsFix() == true).
bool hasGpsFix();
double gpsLatitude();
double gpsLongitude();
uint32_t gpsSatellites();

} // namespace Sensors
