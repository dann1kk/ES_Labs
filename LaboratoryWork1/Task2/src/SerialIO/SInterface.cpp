#include "SInterface.h"
#include "Led/Led.h"

SerialIO::SerialIO()
{
  ledState = LOW;
}

void SerialIO::setup()
{
  Serial.begin(9600);
  Serial.println(askOutput);
}

void SerialIO::waitInput()
{
  while (Serial.available() == 0)
  {
  }
}

void SerialIO::takeInput()
{
  delay(64);
  charsRead = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
  input[charsRead] = 0;
  receivedInput = input;
  Serial.println(receivedInput);
}

bool SerialIO::changeLight()
{
  if (receivedInput == turnOnMessage)
  {
    if (ledState == false)
    {
      ledState = true;
      return true;
    }
    else
    {
      Serial.println(alreadyOn);
    }
  }
  else if (receivedInput == turnOffMessage)
  {
    if (ledState == true)
    {
      ledState = false;
      return false;
    }
    else
    {
      Serial.println(alreadyOff);
    }
  }
  else
    Serial.println(wrongCommand);
}
