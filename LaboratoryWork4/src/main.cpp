#include <Arduino.h>
#include "Relay.h"
#include "Motor.h"

#define MOTOR_IN2 8
#define MOTOR_IN1 9
#define MOTOR_ENABLE 11
#define RELAY_PIN 12

#define RELAY_ON "on"
#define RELAY_OFF "off"
#define RPM_GET "rpm"
#define RPM_CHANGE "motor"
#define TURN_LEFT "left"
#define TURN_RIGHT "right"


Relay relay(RELAY_PIN);
Motor motor(MOTOR_IN1, MOTOR_IN2, MOTOR_ENABLE);

bool interrupt_flag = false;
double rpm = 0;
double frequency = 0;
unsigned long currentMicros = 0;
unsigned long previousMicros = 0;
const int pulsesPerRotation = 24;

const int filterSize = 5;
unsigned long pulseIntervals[filterSize] = {0};
int filterIndex = 0;
unsigned long sumPulseIntervals = 0;

static String userInput;

void rpmInterrupt();

void setup() 
{
  Serial.begin(19200);
  motor.setSpeed(100);
  attachInterrupt(digitalPinToInterrupt(2), rpmInterrupt, RISING);
}

void loop() 
{
  while (Serial.available() == 0) {}
  userInput = Serial.readStringUntil('\r');
  userInput.toLowerCase();

  String command = userInput.substring(0, userInput.indexOf(' '));
  int value = userInput.substring(userInput.indexOf(' ') + 1).toInt();

  if (command == RELAY_ON) 
  {
    relay.on();
  } 
  else if (command == RELAY_OFF) 
  {
    relay.off();
  } 
  else if (command == RPM_CHANGE) 
  {
    motor.setSpeed(value);
  } 
  else if (userInput == RPM_GET) 
  {
    Serial.print("Motor speed is: ");
    Serial.println(motor.getSpeed());
  } 
  else if (command == TURN_LEFT)
  {
    motor.setDirection(-1);
  }
  else if (command == TURN_RIGHT)
  {
    motor.setDirection(1);
  }
  else 
  {
    Serial.println("Command unknown.");
  }
}

// Update the pulse interval filter with the new pulse interval
void updatePulseIntervalFilter(unsigned long pulseInterval) 
{
  // Remove oldest pulse interval from sum and add new pulse interval
  sumPulseIntervals -= pulseIntervals[filterIndex];
  pulseIntervals[filterIndex] = pulseInterval;
  sumPulseIntervals += pulseIntervals[filterIndex];

  // Update filter index for the next sample
  filterIndex = (filterIndex + 1) % filterSize;
}

// Calculate motor RPM based on average pulse interval
void calculateRpm(unsigned long avgPulseInterval) 
{
  // Calculate frequency from average pulse interval (taking half of the interval due to rising and falling edges)
  // frequency = (1.0/(double)(abs(currentMillis-previousMillis)))*1000.0;
  frequency = 1000000.0 / (double)(0.5 * avgPulseInterval);

  // Calculate RPM from frequency and pulses per rotation
  // rpm = 60.0 / ((1.0/frequency) * 24.0);
  rpm = (60.0 * frequency) / (double)pulsesPerRotation;
}

void rpmInterrupt() 
{
  interrupt_flag = !interrupt_flag;

  if (interrupt_flag == 1) 
  {
    currentMicros = micros();
  } 
  else 
  {
    // Calculate pulse interval between current and previous micros() values
    unsigned long pulseInterval = currentMicros - previousMicros;

    updatePulseIntervalFilter(pulseInterval);

    // Calculate average pulse interval
    unsigned long avgPulseInterval = sumPulseIntervals / filterSize;

    if (avgPulseInterval > 0) 
    {
      calculateRpm(avgPulseInterval);
    }

    previousMicros = currentMicros;
  }

  // Update the motor RPM value
  motor.updateRpm(rpm);
}