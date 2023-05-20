#ifndef SINTERFACE_H
#define SINTERFACE_H

#include <Arduino.h>
#include <stdio.h>

class SerialIO
{
private:
    byte ledState;
    String askOutput = "'turn on' to turn on the led, or 'turn off' to turn it off";
    int charsRead;
    char input[10];
    String turnOnMessage = "turn on";
    String alreadyOn = "The led is already on";
    String turnOffMessage = "turn off";
    String alreadyOff = "The led is already off";
    String wrongCommand = "There is no such command";

public:
    String receivedInput;
    SerialIO();
    void setup();
    void waitInput();
    void takeInput();
    bool changeLight();
};

#endif