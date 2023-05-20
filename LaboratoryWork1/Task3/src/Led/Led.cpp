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

void Led::switchLightTimer() 
{
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
}

bool Led::getLedState() 
{
  if (ledState == false) 
  {
      return false;
  } 
  else 
  {
    return true;
  }
}

void Led::setLedState(bool state) 
{
  ledState = state;
}