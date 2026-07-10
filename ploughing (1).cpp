#include "ploughing.h"
#include "config.h"

namespace Ploughing {

enum State { IDLE, LOWERING, RAISING };
static State state = IDLE;

// Safety cap in case the limit switch fails / actuator stalls before reaching it.
static const unsigned long MAX_ACTUATE_MS = 8000;
static unsigned long actuate_start_ms = 0;

void begin() {
    pinMode(PLOUGH_EXTEND_PIN, OUTPUT);
    pinMode(PLOUGH_RETRACT_PIN, OUTPUT);
    pinMode(PLOUGH_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    stop();
}

bool isLimitReached() {
    // Active LOW: switch pulls the pin to GND when the plough reaches depth.
    return digitalRead(PLOUGH_LIMIT_SWITCH_PIN) == LOW;
}

void stop() {
    digitalWrite(PLOUGH_EXTEND_PIN, LOW);
    digitalWrite(PLOUGH_RETRACT_PIN, LOW);
    state = IDLE;
}

void lower() {
    digitalWrite(PLOUGH_RETRACT_PIN, LOW);
    digitalWrite(PLOUGH_EXTEND_PIN, HIGH);
    state = LOWERING;
    actuate_start_ms = millis();
}

void raise() {
    digitalWrite(PLOUGH_EXTEND_PIN, LOW);
    digitalWrite(PLOUGH_RETRACT_PIN, HIGH);
    state = RAISING;
    actuate_start_ms = millis();
}

void update() {
    if (state == IDLE) return;

    unsigned long elapsed = millis() - actuate_start_ms;

    if (state == LOWERING && (isLimitReached() || elapsed > MAX_ACTUATE_MS)) {
        stop();
    } else if (state == RAISING && elapsed > MAX_ACTUATE_MS) {
        // No "fully retracted" sensor in the current hardware plan — timed retract.
        // Replace with a second limit switch if one is added to the build.
        stop();
    }
}

} // namespace Ploughing
