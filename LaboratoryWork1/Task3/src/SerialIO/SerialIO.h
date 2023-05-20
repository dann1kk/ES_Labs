#ifndef SINTERFACE_H
#define SINTERFACE_H

#include <Arduino.h>
#include <stdio.h>

class SerialIO 
{
    private:
        byte ledState;
        String askOutput = "Secret code:";
        String waitOutput = "Waiting for a command: ";
        int charsRead;
        char input[20];
        String turnOnMessage = "turn on";
        String alreadyOn = "The led is already on";
        String turnOffMessage = "turn off";
        String alreadyOff = "The led is already off";
    public:
        String receivedInput;
        String receivedTurnOn = "Message received! Turning on the led";
        String receivedTurnOff = "Message received! Turning off the led";
        String wrongCommand = "There is no such command";
        SerialIO();
        void setup();
        void waitInput();
        void takeInput();
        bool changeLight();
};

#endif