#include "SerialIO/SInterface.h"
#include "Led/Led.h"

#include <Arduino.h>
#include <stdio.h>

const int ledPin = 11;

SerialIO interface;
Led led;

void setup()
{
  led.setPin(ledPin);
  led.setup();
  interface.setup();
}

void loop()
{
  interface.waitInput();
  delay(64);
  interface.takeInput();
  bool result = interface.changeLight();
  if (result == true)
  {
    led.switchLight(true);
  }
  else if (result == false)
  {
    led.switchLight(false);
  }
}