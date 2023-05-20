#include <Arduino.h>

const int buttonPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int numFloors = sizeof(buttonPins) / sizeof(buttonPins[0]);
int currentFloor = 1;

enum State { Floor1, Floor2, Floor3, Floor4, Floor5, Floor6, Floor7, Floor8 };

State transition(State currentState, int input);
void goToFloor(int targetFloor);

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numFloors; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.println("Elevator initialized. Current floor: 1");
}

void loop() {
  for (int i = 0; i < numFloors; i++) {
    if (digitalRead(buttonPins[i]) == LOW) { 
      goToFloor(i + 1);
      delay(50); 
    }
  }
}

State transition(State currentState, int input) {
  const int nextState = static_cast<int>(currentState) + input;
  if (nextState >= 0 && nextState < numFloors) {
    return static_cast<State>(nextState);
  }
  return currentState;
}

void goToFloor(int targetFloor) {
  if (currentFloor != targetFloor) {
    int step = (currentFloor < targetFloor) ? 1 : -1;
    State targetState = static_cast<State>(targetFloor - 1);
    State currentState = static_cast<State>(currentFloor - 1);

    while (currentState != targetState) {
      currentFloor += step;
      currentState = transition(currentState, step);
      Serial.print("Moving to floor: ");
      Serial.println(currentFloor);
      delay(500);
    }
    Serial.print("Reached floor: ");
    Serial.println(currentFloor);
  }
}
