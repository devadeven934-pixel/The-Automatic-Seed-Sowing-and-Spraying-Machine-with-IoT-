#pragma once
#include <Arduino.h>

namespace Drive {

void begin();

// speed range: -255 (full reverse) .. 0 (stop) .. 255 (full forward)
void setSpeeds(int leftSpeed, int rightSpeed);

void forward(int speed = 200);
void reverse(int speed = 200);
void turnLeft(int speed = 180);
void turnRight(int speed = 180);
void stop();

} // namespace Drive
