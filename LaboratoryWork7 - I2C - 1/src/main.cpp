#include <Wire.h>
#include <Arduino.h>

void receivePacket(int byteCount);

// I2C address for MCU2
const byte i2cAddress = 4;

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize I2C communication
    Wire.begin(i2cAddress);
    Wire.onReceive(receivePacket);
}

void loop()
{
    delay(100);
}

void receiveDataOverI2C(int byteCount)
{
    if (byteCount >= sizeof(float))
    {
        float receivedFloat;
        byte *byteArray = (byte *)&receivedFloat;

        for (int i = 0; i < sizeof(float); i++)
        {
            byteArray[i] = Wire.read();
        }

        // Use the received float value
        // Example: print it to the serial monitor
        Serial.print("Received Distance: ");
        Serial.print(receivedFloat, 2); // Print with 4 decimal places
        Serial.println(" cm");
    }
}

const byte distancesArrayLength = 4;
const byte bytes_length = distancesArrayLength * sizeof(float);
const byte packetSize = 13 + bytes_length; // Total packet size including protocol fields

void receivePacket(int byteCount)
{
    Serial.println("------------------------------------------------");

    byte packet[packetSize];

    for (int i = 0; i < packetSize; i++)
    {
        packet[i] = Wire.read();
    }

    // Start Byte verification
    if (packet[0] != 0xAA)
    {
        return; // Invalid packet, discard
    }

    // End Byte verification
    if (packet[1] != 0xBB)
    {
        return; // Invalid packet, discard
    }

    // Checksum calculation
    int receivedChecksum = 0;
    for (int i = 0; i < packetSize - 1; i++)
    {
        receivedChecksum += packet[i];
    }
    receivedChecksum &= 0xFF;

    // Checksum verification
    if (receivedChecksum != packet[packetSize - 1])
    {
        return; // Invalid packet, discard
    }

    // Extracting fields from the packet
    byte packetCounter = packet[2];
    byte senderID = packet[3];
    byte receiverID = packet[4];
    byte packetType = packet[5];
    byte payloadLength = packet[6];



    // Serial.println(payloadLength);



    // Payload (float value)
    float floatArray[payloadLength / sizeof(float)];
    for (int i = 0; i < payloadLength / sizeof(float); i++)
    {
        byte floatb[sizeof(float)]; // the float containing the distance
        floatb[0] = packet[7 + i * sizeof(float)];
        floatb[1] = packet[7 + i * sizeof(float) + 1];
        floatb[2] = packet[7 + i * sizeof(float) + 2];
        floatb[3] = packet[7 + i * sizeof(float) + 3];

        float realFloat;
        memcpy(&realFloat, floatb, sizeof(float));

        floatArray[i] = realFloat;
    }

    // Convert byte array to float
    float receivedFloat;
    memcpy(&receivedFloat, floatArray, sizeof(float));

    // Use the received float value
    // Example: print it to the serial monitor
    // Serial.print("Received Float: ");
    for (int i = 0; i < payloadLength / sizeof(float); i++)
    {
        Serial.println(floatArray[i]);
    }
}
