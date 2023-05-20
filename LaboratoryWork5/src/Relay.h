#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {
public:
    Relay(int pin);
    void on();
    void off();

private:
    int _pin;
};

#endif