#include "CustomKeypad.h"

CustomKeypad::CustomKeypad() {}

Keypad CustomKeypad::customKey()
{
    Keypad customPad = Keypad(makeKeymap(hexKeys), rowPins, colPins, rows, cols);
    return customPad;
}

bool CustomKeypad::checkCode(char arr[]) 
{
    bool isEqualToGreen = true;
    for (int i = 0; i < 4; i++)
    {
        if (arr[i] != greenLedCode[i])
        {
            isEqualToGreen = false;
        }
    }
    if (isEqualToGreen == true) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}