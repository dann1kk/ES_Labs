#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

enum Direction {
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
};

class Motor {
public:
    Motor(int in1, int in2, int enable);
    double getSpeed();
    void setSpeed(int speed);
    void setDirection(Direction direction);
    void updateRpm(double rpm);

private:
    int _in1;
    int _in2;
    int _enable;
    double _rpm;
};

#endif