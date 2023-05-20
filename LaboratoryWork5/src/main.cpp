#include <Arduino.h>
#include <PID_v1.h>
#include <Motor.h>
#include <Relay.h>

#define MOTOR_PIN1 8
#define MOTOR_PIN2 9
#define MOTOR_EN 11
#define RELAY_PIN 12
#define TEMP_PIN A0

static String input;
static int motorSpeedValue;

String readInput();
void helpmessage();
void idleWait();

double rpm = 0;
double rpm_setpoint = 80;
double setpoint = 50;
double comp_speed = 0;
double Kp = 0.3; 
double Ki = 1.2; 
double Kd = 0.01;
double temperature = -159;

unsigned long averageFilter(unsigned long arr[], int size);
double adc_to_voltage(uint8_t analog_in, double voltage_range, double adc_range);
double voltage_to_physical(double voltage);

void rpmInterrupt();
void pidRecompute();
void histerisis();

Relay relay(RELAY_PIN);
Motor motor(MOTOR_PIN2, MOTOR_PIN1, MOTOR_EN);
PID myPID(&rpm, &comp_speed, &rpm_setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);
  attachInterrupt(digitalPinToInterrupt(2), rpmInterrupt, RISING);
}

void handleTerminal(String command) {
  if (command == "get_temp") {
    Serial.print("Temperature is: ");
    Serial.println(temperature);
    Serial.println("Temperature Setpoint: ");
    Serial.println(setpoint);
  } else if (command == "set_temp") {
    Serial.print("Enter temp:");
    setpoint = readInput().toInt();
    Serial.print("Temperature setpoint: ");
    Serial.println(setpoint);
  } else if (command == "get_motor") {
    Serial.print("Motor setpoint: ");
    Serial.println(rpm_setpoint);
    Serial.print("Motor speed: ");
    Serial.println(motor.getSpeed());
  } else if (command == "set_motor") {
    Serial.print("Enter motor speed: ");
    motorSpeedValue = readInput().toInt();
    motor.setSpeed(motorSpeedValue);
    rpm_setpoint = motorSpeedValue;
    Serial.print("Motor speed setpoint set: ");
    Serial.println(motorSpeedValue % 256);
  } 
}

void loop() {
  if (Serial.available()) {
    String command = readInput();
    handleTerminal(command);
  }
}

String readInput() {
  String input = "";
  char c;
  idleWait();
  while (Serial.peek() != '\r' && Serial.peek() != '\n') {
    c = (char)Serial.read();
    Serial.print(c);
    input += c;
    idleWait();
  }
  Serial.read();
  input.toLowerCase();
  Serial.println();
  return input;
}

void idleWait() {
  while (Serial.available() == 0) {
    pidRecompute();
    histerisis();
  }
}

void pidRecompute() {
  myPID.Compute();
  motor.setSpeed(comp_speed);
}

void histerisis() {
  double voltage = adc_to_voltage(TEMP_PIN, 5.0, 1023.0);
  temperature = voltage_to_physical(voltage);

  if (temperature > setpoint) {
    relay.off();
  } else if (temperature < setpoint) {
    relay.on();
  }
}

double adc_to_voltage(uint8_t analog_in, double voltage_range, double adc_range) {
  double adc_value = analogRead(analog_in);
  double voltage = (adc_value * voltage_range) / adc_range;
  return voltage;
}

double voltage_to_physical(double voltage) {
  return voltage * 100;
}

bool interrupt_flag = false;
double frequency = 0;
unsigned long micros1 = 0;
unsigned long micros0 = 0;
const int pulsesPerRotation = 24;

const int pulseIntervalsSize = 5;
int pulseIntervalIndex = 0;
unsigned long pulseIntervals[pulseIntervalsSize] = {0};

void rpmInterrupt() {
  interrupt_flag = !interrupt_flag;

  if (interrupt_flag == 1) {
    micros1 = micros();
  } else {
    unsigned long pulseInterval = micros1 - micros0;
    pulseIntervals[pulseIntervalIndex] = pulseInterval;
    unsigned long avgInterval = averageFilter(pulseIntervals, pulseIntervalsSize);
    pulseIntervalIndex = (pulseIntervalIndex + 1) % pulseIntervalsSize;

    if (avgInterval > 0) {
      frequency = 1000000.0 / (double)(0.5 * avgInterval);
      rpm = (60.0 * frequency) / (double)pulsesPerRotation;
    }
    micros0 = micros1;
  }
  motor.updateRpm(rpm);
}

unsigned long averageFilter(unsigned long arr[], int size) {
  unsigned long sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  return sum /= size;
}