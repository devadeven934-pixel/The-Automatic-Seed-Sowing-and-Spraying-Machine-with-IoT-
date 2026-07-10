#pragma once
#include <Arduino.h>

namespace Spraying {

void begin();
void start();  // turn pump + solenoid on
void stop();   // turn pump + solenoid off
bool isActive();

} // namespace Spraying
