#include "sensors.h"
#include "config.h"
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

namespace Sensors {

static TinyGPSPlus gps;
static HardwareSerial gpsSerial(2); // UART2

void begin() {
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    analogReadResolution(12); // ESP32 ADC: 0-4095

    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

void update() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
}

float readUltrasonicCm() {
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    unsigned long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000UL); // 30ms timeout
    if (duration == 0) {
        return -1.0f; // no echo / out of range
    }
    return (float)duration * 0.0343f / 2.0f;
}

int readSoilMoisturePercent() {
    int raw = analogRead(SOIL_MOISTURE_PIN);
    int percent = map(raw, SOIL_ADC_DRY, SOIL_ADC_WET, 0, 100);
    return constrain(percent, 0, 100);
}

bool hasGpsFix() {
    return gps.location.isValid();
}

double gpsLatitude() {
    return gps.location.isValid() ? gps.location.lat() : 0.0;
}

double gpsLongitude() {
    return gps.location.isValid() ? gps.location.lng() : 0.0;
}

uint32_t gpsSatellites() {
    return gps.satellites.isValid() ? gps.satellites.value() : 0;
}

} // namespace Sensors
