#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(int in1, int in2, int enable);
    void setSpeed(int speed);
    double getSpeed();
    void updateRpm(double rpm);
    void setDirection(int direction);

private:
    int _in1;
    int _in2;
    int _enable;
    double _rpm;
};

#endif
