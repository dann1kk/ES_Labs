#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button 
{
    private:
        int buttonPin;
    public:
        Button();
        void setPin(int pin);
        void setup();
        bool readButton();
};

#endif