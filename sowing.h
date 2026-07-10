#pragma once
#include <Arduino.h>

namespace Sowing {

void begin();

// Call every loop() — reads the rotary encoder and drops a seed every
// SEED_SPACING_CM of travel while sowing is active.
void update();

void start();   // enable automatic seed dropping
void stop();    // disable automatic seed dropping
bool isActive();

// Manually trigger a single seed drop (used for testing / manual mode).
void dropSeed();

// Total distance travelled since boot, in cm (from the wheel encoder).
float distanceTravelledCm();

} // namespace Sowing
