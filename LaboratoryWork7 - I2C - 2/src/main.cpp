#include <Wire.h>
#include <math.h>
#include <Arduino.h>

const int triggerPin = 13;
const int echoPin = 12;

// I2C address for MCU2
const byte i2cAddress = 4;
// Ultrasonic sensor variables
float distance;

float readUltrasonicSensor();
void sendPacket(byte* bytes, byte bytes_length);

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize I2C communication
    Wire.begin();
}

int index = 0;
const int distancesArrayLength = 4;
float distancesArray[distancesArrayLength];

void loop()
{
    // Read the distance from the ultrasonic sensor
    distance = readUltrasonicSensor();
    
    distancesArray[index] = distance;
    index += 1;

    if(index == distancesArrayLength) {
        // Send the distance over I2C to MCU2
        byte bytesLength = distancesArrayLength * sizeof(float);
        byte bytes[bytesLength];
        for(int i = 0; i < distancesArrayLength; ++i) {
            byte* floatBytes = (byte*)&distancesArray[i];
            bytes[i * sizeof(float) + 0] = floatBytes[0];
            bytes[i * sizeof(float) + 1] = floatBytes[1];
            bytes[i * sizeof(float) + 2] = floatBytes[2];
            bytes[i * sizeof(float) + 3] = floatBytes[3];
        }
        sendPacket(bytes, bytesLength);
    }

    // Delay before the next reading
    delay(500);
    index %= distancesArrayLength;
}

float readUltrasonicSensor()
{
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    long duration = pulseIn(echoPin, HIGH);

    // Calculate the distance in centimeters
    float distance_cm = duration * 0.034 / 2;

    // Print the distance
    // Serial.print("Distance: ");
    Serial.println(distance_cm);
    // Serial.println(" cm");

    return distance_cm;
}

void sendDataOverI2C(float data)
{
    byte *byteArray = (byte *)&data;
    Wire.beginTransmission(i2cAddress);
    for (int i = 0; i < sizeof(float); i++)
    {
        Wire.write(byteArray[i]);
    }
    Wire.endTransmission();
}

void sendPacket(byte* bytes, byte bytes_length)
{
    byte packetSize = 13 + bytes_length; // Total packet size including protocol fields
    byte packet[packetSize];

    // Start Byte
    packet[0] = 0xAA;

    // End Byte
    packet[1] = 0xBB;

    // Packet Counter
    static byte packetCounter = 0;
    packet[2] = packetCounter++;

    // Sender ID
    packet[3] = 0x01; // Example ID for MCU1

    // Receiver ID
    packet[4] = 0x02; // Example ID for MCU2

    // Packet Type
    packet[5] = 0x01; // Example packet type

    // Packet Length
    packet[6] = bytes_length;

    // Payload (float value)
    // byte *floatArray = (byte *)&value;
    for (byte i = 0; i < bytes_length; i++)
    {
        packet[7 + i] = bytes[i];
    }

    // Checksum calculation
    int checksum = 0;
    for (int i = 0; i < packetSize - 1; i++)
    {
        checksum += packet[i];
    }
    packet[packetSize - 1] = checksum & 0xFF;

    // Send the packet over I2C to MCU2
    Wire.beginTransmission(i2cAddress);
    Wire.write(packet, packetSize);
    Wire.endTransmission();
}