#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led 
{
    private:
        int ledPin;
    public:
        Led();
        void setPin(int pin);
        void setup();
        void switchLight(bool ledState);
};

#endif