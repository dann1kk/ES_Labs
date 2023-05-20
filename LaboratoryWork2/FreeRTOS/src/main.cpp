#include <Arduino_FreeRTOS.h>
#include <HardwareSerial.h>
#include <Arduino.h>
#include <semphr.h>
#define BUTTON_PIN 13
#define BUTTON_LED_INCR 10
#define BUTTON_LED_DECR 11
#define LED_PIN 4
#define LED_BLINK_PIN 5
#define LED_ON 1
#define LED_OFF 0
#define TASK_REC 2
#define TASK_OFFSET 5
int led2Rec;
int ledState;
int led2State;
int buttonState;
int led2ReadRec;
int btnResetState;
SemaphoreHandle_t xSerialSemaphore;

void GreenLEDblink(void *pvParameters);
void IncDecRecLED(void *pvParameters);
void PrintStates(void *pvParameters);
void RedLEDstatus(void *pvParameters);
void Reset(void *pvParameters);
void setup()
{
  led2Rec = 0;
  ledState = 0;
  led2State = 0;
  buttonState = 0;
  led2ReadRec = 1000;
  pinMode(BUTTON_LED_DECR, INPUT);
  pinMode(BUTTON_LED_INCR, INPUT);
  pinMode(LED_BLINK_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(9, INPUT);
  Serial.begin(9600);
  if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
  xTaskCreate(GreenLEDblink, "GreenLedBlink", 128, NULL, 3, NULL);
  xTaskCreate(IncDecRecLED, "RecForLed", 128, NULL, 2, NULL);
  xTaskCreate(PrintStates, "Print", 128, NULL, 3, NULL);
  xTaskCreate(RedLEDstatus, "RedLED", 128, NULL, 1, NULL);
  xTaskCreate(Reset, "ResetEverything", 128, NULL, 3, NULL);
  vTaskStartScheduler();
}
void loop()
{
}
void GreenLEDblink(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE )
    {
      ledState = !ledState;
      digitalWrite(LED_BLINK_PIN, ledState);

      xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
    }

    vTaskDelay(led2ReadRec / portTICK_PERIOD_MS);
  }
}
void IncDecRecLED(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if (digitalRead(BUTTON_LED_INCR) == 0)
    {
      led2ReadRec = led2ReadRec + 1000;
    }
    if (digitalRead(BUTTON_LED_DECR) == 0)
    {
      led2ReadRec = led2ReadRec - 1000;
    }
    vTaskDelay(15);
  }
}
void PrintStates(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    Serial.println();
    Serial.print("Red LED status:");
    if (ledState == 1)
    {
      Serial.println("ON");
    }
    else
      Serial.println("OFF");
    Serial.print("Green LED status:");
    if (led2State == 1)
    {
      Serial.println("ON");
    }
    else
      Serial.println("OFF");
    Serial.print("Green LED rec:");
    Serial.print(led2ReadRec / 1000);
    Serial.println("s");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void RedLEDstatus(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    if (digitalRead(BUTTON_PIN) == 0)
    {
      led2State = !led2State;
      digitalWrite(LED_PIN, led2State);
      while(digitalRead(BUTTON_PIN) == 0);
    }
    if (led2State == 1)
    {
      if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ) 
      {
        // handle exception
      }
    }
    else
    {
      xSemaphoreGive( xSerialSemaphore );
    }
 
    vTaskDelay(1);
  }
}
void Reset(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    btnResetState = digitalRead(9);
    if (btnResetState == 0)
    {
      led2Rec = 0;
      
      digitalWrite(LED_PIN, LOW);
      digitalWrite(LED_BLINK_PIN, LOW);
      led2ReadRec = 1000;
      Serial.println("System Reseted!");
    }
    vTaskDelay(35);
  }
}