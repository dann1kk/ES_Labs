#include "Motor.h"

Motor::Motor(int in1, int in2, int enable) : _in1(in1), _in2(in2), _enable(enable) {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_enable, OUTPUT);

    // Set initial motor rotation clockwise
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
}

void Motor::setSpeed(int speed) {
    analogWrite(_enable, speed);
    Serial.print("Motor speed set to: ");
    Serial.println(speed % 256);
}

double Motor::getSpeed() {
    return _rpm;
}

void Motor::updateRpm(double rpm) {
    _rpm = rpm;
}

void Motor::setDirection(int direction) {
  if (direction == 1) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
  } else if (direction == -1) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
  }
}
