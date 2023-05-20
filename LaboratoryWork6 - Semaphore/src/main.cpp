#include <Arduino.h>

const int redLED1 = 5;
const int yellowLED1 = 4;
const int greenLED1 = 3;

const int redLED2 = 10;
const int yellowLED2 = 9;
const int greenLED2 = 8;

const unsigned long redDelay = 2000;
const unsigned long yellowDelay = 1000;
const unsigned long greenDelay = 3000;

enum SemaphoreState { RED, YELLOW, GREEN };
SemaphoreState state1 = RED;
SemaphoreState state2 = GREEN;

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long interval1 = redDelay;
unsigned long interval2 = redDelay;

void updateSemaphoreState(int redLED, int yellowLED, int greenLED, SemaphoreState& state, unsigned long& previousMillis, unsigned long& interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    switch (state) {
      case RED:
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
        state = GREEN;
        interval = greenDelay;
        Serial.println("State: RED");
        break;
      case YELLOW:
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
        state = RED;
        interval = redDelay;
        Serial.println("State: YELLOW");
        break;
      case GREEN:
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, HIGH);
        state = YELLOW;
        interval = yellowDelay;
        Serial.println("State: GREEN");
        break;
    }
    previousMillis = currentMillis;
  }
}

void setup() {
  pinMode(redLED1, OUTPUT);
  pinMode(yellowLED1, OUTPUT);
  pinMode(greenLED1, OUTPUT);
  pinMode(redLED2, OUTPUT);
  pinMode(yellowLED2, OUTPUT);
  pinMode(greenLED2, OUTPUT);
}

void loop() {
  updateSemaphoreState(redLED1, yellowLED1, greenLED1, state1, previousMillis1, interval1);
  updateSemaphoreState(redLED2, yellowLED2, greenLED2, state2, previousMillis2, interval2);
}
