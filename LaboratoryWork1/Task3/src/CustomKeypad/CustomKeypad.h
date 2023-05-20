#ifndef CUSTOMKEYPAD_H
#define CUSTOMKEYPAD_H

#include <Arduino.h>
#include <Keypad.h>

class CustomKeypad
{
    private:
        char greenLedCode[4] = {'1', '1', '1', '1'};
        const byte rows = 4;
        const byte cols = 4;
        char hexKeys[4][4] = {
            {'7', '8', '9', '/'},
            {'4', '5', '6', '*'},
            {'1', '2', '3', '-'},
            {'C', '0', '=', '+'}};
        byte rowPins[4] = {8, 9, 10, 11};
        byte colPins[4] = {2, 3, 4, 5};
    public:
        CustomKeypad();
        Keypad customKey();
        bool checkCode(char arr[]);
};

#endif