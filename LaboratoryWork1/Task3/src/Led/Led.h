#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led 
{
    private:
        int ledPin;
        byte ledState = LOW;
    public:
        Led();
        void setPin(int pin);
        void setup();
        void switchLight(bool ledState);
        void switchLightTimer();
        bool getLedState();
        void setLedState(bool state);
};

#endif