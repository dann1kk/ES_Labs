#include "Led.h"

Led::Led() {}

void Led::setPin(int pin) 
{
  ledPin = pin;
}

void Led::setup() 
{
  pinMode(ledPin, OUTPUT);
}

void Led::switchLight(bool ledState) 
{
    bool state = ledState;
    if (state == true) 
    {
      digitalWrite(ledPin, HIGH);
    } else 
    {
      digitalWrite(ledPin, LOW);
    }
}