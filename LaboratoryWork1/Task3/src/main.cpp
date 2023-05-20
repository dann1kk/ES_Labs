#include "CustomKeypad/CustomKeypad.h"
#include "SerialIO/SerialIO.h"
#include "Led/Led.h"

#include <Arduino.h>
#include <stdio.h>

const int redLedPin = 13;
const int greenLedPin = 12;

int lastNumber = 0;
char codeArray[4];

CustomKeypad keypad;
Keypad customKeypad = keypad.customKey();
SerialIO interface;
Led redLed;
Led greenLed;

void setup() 
{
  interface.setup();
  redLed.setPin(redLedPin);
  redLed.setup();
  greenLed.setPin(greenLedPin);
  greenLed.setup();
}

void loop() 
{
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    if (lastNumber < 4) 
    {
      Serial.print(customKey);
      codeArray[lastNumber] = customKey;
      lastNumber++;
      if (lastNumber == 4)
      {
        bool checkResult = keypad.checkCode(codeArray);
        if (checkResult == true)
        {
          Serial.println("\n");
          Serial.println("\nCode is correct!");
          greenLed.switchLightTimer();
        }  
        else if (checkResult == false)
        {
          Serial.println("\n");
          Serial.println("Code is incorrect!");
          redLed.switchLightTimer();
        }
        strcpy(codeArray, "");
        lastNumber = 0;
      }
    }
  }
}