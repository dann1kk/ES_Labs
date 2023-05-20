#ifndef SEMAPHORE_H
#define SEMAPHORE_H

enum SemaphoreState { Green, Yellow, Red, YellowToRed };

class Semaphore {
private:
    SemaphoreState state;
    int greenLedPin;
    int yellowLedPin;
    int redLedPin;
public:
    Semaphore(SemaphoreState state, int greenLedPin, int yellowLedPin, int redLedPin);
    ~Semaphore();

    SemaphoreState getState();
    void displayState();
    void setState(SemaphoreState state);
    void changeState();
    void turnLed();
};

#endif