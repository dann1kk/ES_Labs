#include "Motor.h"

Motor::Motor(int in1, int in2, int enable) : _in1(in1), _in2(in2), _enable(enable) {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_enable, OUTPUT);

    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
}

double Motor::getSpeed() {
    return _rpm;
}

void Motor::setDirection(Direction direction) {
  if (direction == Direction::CLOCKWISE) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
  } else if (direction == Direction::COUNTERCLOCKWISE) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
  }
}

void Motor::setSpeed(int speed) {
    analogWrite(_enable, speed);
}

void Motor::updateRpm(double rpm) {
    _rpm = rpm;
}