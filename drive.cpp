#include "drive.h"
#include "config.h"

namespace Drive {

static void applyMotor(int in1Pin, int in2Pin, int pwmChannel, int speed) {
    speed = constrain(speed, -255, 255);

    if (speed > 0) {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
    } else if (speed < 0) {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);
    } else {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, LOW);
    }

    ledcWrite(pwmChannel, abs(speed));
}

void begin() {
    pinMode(LEFT_MOTOR_IN1, OUTPUT);
    pinMode(LEFT_MOTOR_IN2, OUTPUT);
    pinMode(RIGHT_MOTOR_IN3, OUTPUT);
    pinMode(RIGHT_MOTOR_IN4, OUTPUT);

    ledcSetup(LEFT_PWM_CHANNEL, PWM_FREQ_HZ, PWM_RESOLUTION);
    ledcAttachPin(LEFT_MOTOR_PWM, LEFT_PWM_CHANNEL);

    ledcSetup(RIGHT_PWM_CHANNEL, PWM_FREQ_HZ, PWM_RESOLUTION);
    ledcAttachPin(RIGHT_MOTOR_PWM, RIGHT_PWM_CHANNEL);

    stop();
}

void setSpeeds(int leftSpeed, int rightSpeed) {
    applyMotor(LEFT_MOTOR_IN1, LEFT_MOTOR_IN2, LEFT_PWM_CHANNEL, leftSpeed);
    applyMotor(RIGHT_MOTOR_IN3, RIGHT_MOTOR_IN4, RIGHT_PWM_CHANNEL, rightSpeed);
}

void forward(int speed) { setSpeeds(speed, speed); }
void reverse(int speed) { setSpeeds(-speed, -speed); }
void turnLeft(int speed) { setSpeeds(-speed, speed); }
void turnRight(int speed) { setSpeeds(speed, -speed); }
void stop() { setSpeeds(0, 0); }

} // namespace Drive
