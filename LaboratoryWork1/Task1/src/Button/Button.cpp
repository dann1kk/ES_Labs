#include "Button.h"

Button::Button() {}

void Button::setPin(int pin) 
{
  buttonPin = pin;
}

void Button::setup() 
{
  pinMode(buttonPin, INPUT);
}

bool Button::readButton() 
{
    if (digitalRead(buttonPin) == HIGH) 
    {
      return true;
    } else 
    {
      return false;
    }
}