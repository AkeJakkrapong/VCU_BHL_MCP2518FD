/*#####       LED         #####*/
#define  pinLED 2
/*******************************/
/*#####       SPI         #####*/
#define  SPI_CS_PIN 5
/*******************************/
/*#####   Digital Input   #####*/
#define pinInput1 25
#define pinInput2 26
/*******************************/
/*#####   Digital Output  #####*/
#define pinRelay1 27
#define pinRelay2 4
#define pinBuzzer 22
/*******************************/
/*#####    Analog Input   #####*/
#define pinA1 32
#define pinA2 33
/*******************************/
/*##### Acceletor Input   #####*/
#define pinAcc 34
/*******************************/
/*#####         PWM       #####*/
#define pinPWM1 13
#define pinPWM2 14
/*******************************/

#include <CANBus-SOLDERED.h>
CANBus CAN(SPI_CS_PIN);

//##################  Function Control Relay  ################## 
void Relay(uint8_t ch, byte state)
{
  switch (ch)
  {
    case 1:
          digitalWrite(pinRelay1,state);
          break;
    case 2:
          digitalWrite(pinRelay2,state);
          break;
  }
}

//##################    Function Read Volt    ################## 
uint16_t readVolt(uint8_t analogPin)
{
  float v=0;
  for(uint8_t i=0;i<10;i++)
  {
    v = v + (analogRead(analogPin)*3.3/4095.0);
  }
  v = v/10.0;
  if(v < 0.07)
  {
    v = 0;
  }
  else
  {
    v = 0.04923616*pow(v,3) - 0.18949336*pow(v,2) + 1.78652637*v + 0.21060003;
  }
  uint16_t vx100 = v*100;
  return vx100;
}


// ESP32 Board v3.1.3
//##################       Function PWM       ##################
void pwm(uint8_t ch, uint16_t freq, uint8_t duty)
{
  uint8_t pwm_resolution = 8;
  switch (ch)
  {
    case 1:
      ledcDetach(pinPWM1);
      ledcAttachChannel(pinPWM1, freq, pwm_resolution, ch);
      ledcWrite(pinPWM1, map(duty, 0, 100, 255, 0));
    break;
    case 2:
      ledcDetach(pinPWM2);
      ledcAttachChannel(pinPWM2, freq, pwm_resolution, ch);
      ledcWrite(pinPWM2, map(duty, 0, 100, 255, 0));
    break;
  } 
}

//// ESP32 Board v2.0
////##################       Function PWM       ##################
//void pwm(uint8_t ch, uint16_t freq, uint8_t duty)
//{
//  uint8_t pwm_resolution = 8;
//  switch (ch)
//  {
//    case 1:
//      ledcSetup(ch, freq, pwm_resolution);
//      ledcAttachPin(pinPWM1, ch);
//      ledcWrite(pinPWM1, map(duty, 0, 100, 255, 0));
//    break;
//    case 2:
//      ledcSetup(ch, freq, pwm_resolution);
//      ledcAttachPin(pinPWM2, ch);
//      ledcWrite(pinPWM2, map(duty, 0, 100, 255, 0));
//    break;
//  } 
//}


//##################    Function Error Code   ################## 
uint8_t error_code = 0;
void error_task(void * parameters)
{
  pinMode(pinBuzzer,OUTPUT);
  for(;;)
  {
    if(error_code!=0)
    {
      for(uint8_t i=0;i<error_code;i++)
      {
        digitalWrite(pinBuzzer,HIGH);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(pinBuzzer,LOW);
        vTaskDelay(200 / portTICK_PERIOD_MS);
      }
      vTaskDelay(1690 / portTICK_PERIOD_MS);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}


//##################  Function LED Blink    ################## 
void LEDblink_task(void * parameters)
{
  pinMode(pinLED,OUTPUT);
  for(;;)
  {
    digitalWrite(pinLED, LOW);     // Built in LED -> ON
    vTaskDelay(200 / portTICK_PERIOD_MS);
    digitalWrite(pinLED, HIGH);     // Built in LED -> OFF
    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}
