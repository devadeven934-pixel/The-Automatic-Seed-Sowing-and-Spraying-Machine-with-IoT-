#pragma once
#include <Arduino.h>

namespace Ploughing {

void begin();

// Non-blocking state update — call every loop().
void update();

void lower();   // extend actuator to till depth (stops automatically at limit switch)
void raise();   // retract actuator to travel position
void stop();    // immediately halt actuator movement

bool isLimitReached();

} // namespace Ploughing
