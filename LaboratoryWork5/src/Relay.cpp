#include "Relay.h"

Relay::Relay(int pin) : _pin(pin) {
    pinMode(_pin, OUTPUT);
}

void Relay::on() {
    digitalWrite(_pin, HIGH);
}

void Relay::off() {
    digitalWrite(_pin, LOW);
}