#include <Arduino.h>
#include "timer-api.h"
#include <stdio.h>
#define BUTTON_PIN 13
#define BUTTON_LED_INCR 10
#define BUTTON_LED_DECR 11
#define BUTTON_PRESSED 0
#define BUTTON_NOT_PRESSED 1
#define LED_PIN 4
#define LED_BLINK_PIN 5
#define LED_ON 1
#define LED_OFF 0
#define TASK_REC 2
#define TASK_OFFSET 5
#define BUTTON1 9
int led2Rec = 0;
int ledState = 0;
int led2State = 0;
int buttonState = 0;
int led2ReadRec = 1;
int btnResetState;

void setup()
{
    // put your setup code here, to run once:
    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_BLINK_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUTTON_LED_DECR, INPUT);
    pinMode(BUTTON_LED_INCR, INPUT);
    pinMode(BUTTON1, INPUT);
    // частота=2Гц, период=500мс
    timer_init_ISR_2Hz(TIMER_DEFAULT);
    Serial.begin(9600);
    led2Rec = TASK_OFFSET;
}

void taskLedWithButton() 
{
    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == BUTTON_PRESSED)
    {
        if (ledState == 1)
        {
            ledState = 0;
        }
        else
        {
            ledState = 1;
            digitalWrite(LED_BLINK_PIN, LOW);
        }
        digitalWrite(LED_PIN, ledState);
    }
}

void taskLedBlink() 
{
    if (ledState == 0)
    {
        if (led2State == 0)
        {
            led2State = 1;
            digitalWrite(LED_BLINK_PIN, HIGH);
        }
        else
        {
            led2State = 0;
            digitalWrite(LED_BLINK_PIN, LOW);
        }
    }
}

void taskButton2LedReadRec() 
{
    if (digitalRead(BUTTON_LED_INCR) == BUTTON_PRESSED)
    {
        led2ReadRec++;
    }
    if (digitalRead(BUTTON_LED_DECR) == BUTTON_PRESSED)
    {
        led2ReadRec--;
    }
}

void Reset()
{
  {
    btnResetState = digitalRead(9);
    if (btnResetState == 0)
    {
      led2Rec = 0;
      ledState = 0;
      led2State = 0;
      digitalWrite(LED_PIN, LOW);
      digitalWrite(LED_BLINK_PIN, LOW);
      led2ReadRec = 1;
      Serial.println("System Reset!");
    }
  }
}

void timer_handle_interrupts(int timer)
{
    taskLedWithButton();
    taskButton2LedReadRec();
    if (--led2Rec <= 0)
    {
        taskLedBlink();
        led2Rec = led2ReadRec;
    }
    Reset();
}

void loop()
{
    Serial.println();
    Serial.print("Red LED status: ");
    if (ledState == 1)
    {
      Serial.println("ON");
    }
    else
      Serial.println("OFF");
    Serial.print("GREEN LED status: ");
    if (led2State == 1)
    {
      Serial.println("OFF");
    }
    else
      Serial.println("ON");
    Serial.print("Green LED recurrency: ");
    Serial.print(led2ReadRec);
    Serial.println("s");
    delay(1000);
    // put your main code here, to run repeatedly:
}