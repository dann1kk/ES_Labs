#include <DHT.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

const int TMP36_PIN = A0;
const int LDR_PIN = A1;
const int DHT_PIN = 2;
const int ULTRASONIC_ECHO_PIN = 3;
const int ULTRASONIC_TRIG_PIN = 4;
const int BMP180_ADDRESS = 0x77;

DHT dht(DHT_PIN, DHT11);
Ultrasonic ultrasonic(ULTRASONIC_ECHO_PIN, ULTRASONIC_TRIG_PIN);
Adafruit_BMP085 bmp;

float weightedMedian(float values[], int size) {
  // Sort the array in ascending order
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if (values[j] < values[i]) {
        float temp = values[i];
        values[i] = values[j];
        values[j] = temp;
      }
    }
  }
  
  // Compute the weighted median
  float median;
  float totalWeight = 0;
  float halfTotalWeight = 0;
  for (int i = 0; i < size; i++) {
    totalWeight += i + 1;
  }
  halfTotalWeight = totalWeight / 2;
  for (int i = 0; i < size; i++) {
    halfTotalWeight -= i + 1;
    if (halfTotalWeight <= 0) {
      median = values[i];
      break;
    }
  }
  
  return median;
}

void setup() {
  pinMode(TMP36_PIN, INPUT);
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT); 
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  dht.begin();
  Wire.begin();
  Serial.begin(9600);
  if (!bmp.begin(BMP180_ADDRESS)) {
    Serial.println("Could not find a valid BMP180 sensor");
    while (1) {}
  }
}

void loop() {
  // Read TMP36 temperature sensor
  float tmp36Raw = analogRead(TMP36_PIN);
  float tmp36Voltage = tmp36Raw * 5.0 / 1023;
  float tmp36Temperature = (tmp36Voltage - 0.5) * 100;

  // Read LDR light sensor
  int ldrRaw = analogRead(LDR_PIN);
  const int numSamples = 5; // Number of samples to take for the filter
  float samples[numSamples];
  for (int i = 0; i < numSamples; i++) {
    samples[i] = ldrRaw;
  }
  float filteredDistance = weightedMedian(samples, numSamples);

  // Read DHT11 temperature and humidity sensor
  float dhtHumidity = dht.readHumidity();
  float dhtTemperature = dht.readTemperature();

  // torched light, ultrasonic
  // const int numSamples = 5; // Number of samples to take for the filter
  // float samples[numSamples];
  // for (int i = 0; i < numSamples; i++) {
  //   digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  //   delayMicroseconds(2);
  //   // Sets the trigPin on HIGH state for 10 micro seconds
  //   digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  //   delayMicroseconds(10);
  //   digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  //   // Reads the echoPin, returns the sound wave travel time in microseconds
  //   float duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  //   // Calculating the distance
  //   float distance= duration*0.034/2;
  //   samples[i] = distance;
  //   delay(10); // Delay between each sample to reduce noise
  // }
  // float filteredDistance = weightedMedian(samples, numSamples);

  // Read BMP180 pressure sensor
  float pressure = bmp.readPressure() / 100.0F;

  // Print sensor data to Serial Monitor
  // Serial.print("TMP36 Temperature: ");
  // Serial.print(tmp36Temperature);
  // Serial.println(" C");
  Serial.print("LDR Value: ");
  Serial.println(filteredDistance);
  // Serial.print("DHT11 Humidity: ");
  // Serial.print(dhtHumidity);
  // Serial.println(" %");
  // Serial.print("DHT11 Temperature: ");
  // Serial.print(dhtTemperature);
  // Serial.println(" C");
  // Serial.print("Ultrasonic Distance: ");
  // Serial.print(filteredDistance);
  // Serial.println(" cm  ");
  // Serial.print("BMP180 Pressure: ");
  // Serial.print(pressure);
  // Serial.println(" hPa");
  // Serial.println("\n");
  
  delay(300);
}