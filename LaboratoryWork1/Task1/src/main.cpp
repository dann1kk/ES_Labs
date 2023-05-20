#include "Button/Button.h"
#include "Led/Led.h"

#include <Arduino.h>

const int buttonPin = 7; 
const int ledPin = 13;

byte lastButtonState = false;
byte ledState = LOW;

Button button;
Led led;

void setup() 
{
  button.setPin(buttonPin);
  led.setPin(ledPin);
  button.setup();
  led.setup();
}

void loop() 
{
  bool buttonState = button.readButton();
  if (buttonState != lastButtonState) 
  {
    lastButtonState = buttonState;
    if (buttonState == false) 
    {
      if (ledState == true) 
      {
        ledState = false;
      } else 
      {
        ledState = true;
      }
      led.switchLight(ledState);
    }
  }
}