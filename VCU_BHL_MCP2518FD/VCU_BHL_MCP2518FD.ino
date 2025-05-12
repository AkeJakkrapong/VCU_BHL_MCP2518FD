/***************************************************************
Msg1 -> ID: 0x150050A1    [control use ID:0x1500A150]
        dataMsg1[0]   0, 0, 0, Relay1  | 0, 0, 0, Relay2
        dataMsg1[1]   error code
        dataMsg1[2]   Freq PWM1 LOW  Byte
        dataMsg1[3]   Freq PWM1 HIGH Byte
        dataMsg1[4]   Duty PWM1
        dataMsg1[5]   Freq PWM2 LOW  Byte
        dataMsg1[6]   Freq PWM2 HIGH Byte
        dataMsg1[7]   Duty PWM2

Msg2 -> ID: 0x150050A2
        dataMsg2[0]   0, 0, 0, dINPUT1 | 0, 0, 0, dINPUT2
        dataMsg2[1]   // error code
        dataMsg2[2]   // Volt Acceletor LOW  Byte
        dataMsg2[3]   // Volt Acceletor HIGH Byte
        dataMsg2[4]   // Volt AnalogInput1  LOW  Byte
        dataMsg2[5]   // Volt AnalogInput1  HIGH Byte
        dataMsg2[6]   // Volt AnalogInput2  LOW  Byte
        dataMsg2[7]   // Volt AnalogInput2  HIGH Byte 
***************************************************************/
#include "VCU_BHL_MCP2518FD.h"
#include "Dashboard_E650.h"

uint16_t cycle1 = 100;
uint16_t cycle2 = 500;
uint32_t last_cycle1;
uint32_t last_cycle2;
uint8_t dataMsg1[8];
uint8_t dataMsg2[8];
uint8_t disp = 0;
byte debug1 = 0;
byte debug2 = 0;


void setup()
{
  Serial.begin(115200);
  delay(500);
  
  xTaskCreate(&LEDblink_task, "LEDblink_task", 1024, NULL, 0, NULL);
  xTaskCreate(&error_task, "error_task", 2048, NULL, 1, NULL);
  
  pinMode(pinRelay1,OUTPUT);
  pinMode(pinRelay2,OUTPUT);

  pinMode(pinInput1,INPUT);
  pinMode(pinInput2,INPUT);

  pwm(1, 1000, 0);
  pwm(2, 1000, 0);
  
  while (CAN_OK != CAN.begin(CAN_500KBPS, MCP2518FD_40MHz))
  {
    error_code = 2;
    Serial.println("CAN init fail!");
    delay(100);
    while(1);
  }
  Serial.println("CAN init OK!");
  last_cycle1 = millis();
  last_cycle2 = millis();
}


void loop() 
{
  if(Serial.available() > 0)
  {
    String sPort = Serial.readStringUntil('\r').substring(0,5);
    if(sPort == "debug")
    {
      debug1 = 1;
      debug2 = 1;
      sPort = "";
      Serial.flush();
    }
    else if(sPort == "kill0")
    {
      debug1 = 0;
      debug2 = 0;
      sPort = "";
      Serial.flush();
    }
    else if(sPort == "kill1")
    {
      debug1 &= 0;
      debug2 &= 1;
      sPort = "";
      Serial.flush();
    }
    else if(sPort == "kill2")
    {
      debug1 &= 1;
      debug2 &= 0;
      sPort = "";
      Serial.flush();
    }  
  }
  uint8_t len = 0;
  uint8_t buf[8];
  if(CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(&len, buf);
    uint32_t id = CAN.getCanId(); // Get ID
    if(debug1)
    {
      Serial.print("ID: 0x"+String(id,HEX));
      Serial.print(" [" +(String)len+"] >> ");
      for (int i = 0; i < len; i++)
      {
        char xBUF[4];
        sprintf(xBUF, "%02X", buf[i]);
        Serial.print(xBUF);
        Serial.print("  "); 
      }
      Serial.println();
    }
    if(id == 0x1500A150)
    {
      for(uint8_t i=0;i<8;i++)
      {
        dataMsg1[i] = buf[i];
      }      
      Relay(1, (dataMsg1[0] >> 4) & 0x01);
      Relay(2, dataMsg1[0] & 0x01);
      error_code = dataMsg1[1];    
      if(dataMsg1[4] != 0)
      {
        pwm(1, dataMsg1[3] << 8 | dataMsg1[2], dataMsg1[4]);
      }
      else
      {
        pwm(1, 1000, 0); 
      }
      if(dataMsg1[7] != 0)
      {
        pwm(2, dataMsg1[6] << 8 | dataMsg1[5], dataMsg1[7]);
      }
      else
      {
        pwm(2, 1000, 0);
      }
    }  
  }  

  byte dINPUT1 = digitalRead(pinInput1);
  byte dINPUT2 = digitalRead(pinInput2);
  uint16_t vAcc = readVolt(pinAcc);
  uint16_t vA1 = readVolt(pinA1);
  uint16_t vA2 = readVolt(pinA2);
  if(debug2)
  {
    if(disp == cycle1)
    {
      Serial.println("D1:"+(String)dINPUT1+",D2:"+(String)dINPUT2+",Acc:"+String(vAcc/100.0,2)+",A1:"+String(vA1/100.0,2)+",A2:"+String(vA2/100.0,2)+",PWM1:"+String(dataMsg1[3] << 8 | dataMsg1[2])+"|"+(String)dataMsg1[4]+"%"+",PWM2:"+String(dataMsg1[6] << 8 | dataMsg1[5])+"|"+(String)dataMsg1[7]+"%");
    }
    disp++;
    if(disp > cycle1)
    {
      disp = 0;
    }
  }
  dataMsg2[0] = (dINPUT1 << 4) | dINPUT2;     // 0, 0, 0, dINPUT1 | 0, 0, 0, dINPUT2
  dataMsg2[1] = error_code;                   // error code
  dataMsg2[2] = vAcc & 0xFF;                  // Volt Acceletor LOW  Byte
  dataMsg2[3] = vAcc >> 8;                    // Volt Acceletor HIGH Byte
  dataMsg2[4] = vA1 & 0xFF;                   // Volt AnalogInput1  LOW  Byte
  dataMsg2[5] = vA1 >> 8;                     // Volt AnalogInput1  HIGH Byte
  dataMsg2[6] = vA2 & 0xFF;                   // Volt AnalogInput2  LOW  Byte
  dataMsg2[7] = vA2 >> 8;                     // Volt AnalogInput2  HIGH Byte 
  if(millis() - last_cycle1 >= cycle1)
  {    
    last_cycle1 = millis();
    CAN.sendMsgBuf(0x150050A1, 1, 8, dataMsg1);
    CAN.sendMsgBuf(0x150050A2, 1, 8, dataMsg2);  
  }
  
  if(millis() - last_cycle2 >= cycle2)
  {  
    last_cycle2 = millis();
    set_E650_Dashboard_Gauge(1.0, 9900, 19, 500.0, -500.0, 10, 100);    // Speed(0 to 199km/h), RPM(0 to 9900rpm), Fuel(0 to 100%), Voltage(0 to 500V), Current(-500 to 500A), Range(0 to 500km), Power(0 to 100%)
    set_E650_Dashboard_Gear('N', 1, 1);                                 // Gear('R', 'N', 'D', 'S'), Gear_effective_position, Run_Preparation
    set_E650_Dashboard_TurnSignal(1, 1);                                // turnLeft, turnRight
    set_E650_Dashboard_Light(1, 1, 1);                                  // position_light, high_beam, rear_fog
    set_E650_Dashboard_Door_alarm(1, 1, 1, 1);                          // Door1, Door2, Door3, Door4
    set_E650_Dashboard_Alarm(1, 1, 1, 1, 1, 1, 2);                      // Park brake, ESP, Power reduction, Battery cut-off, ABS, Charging cable, Charge_status(0:Off, 1:Flashing, 2:Solid On)
    set_E650_Dashboard_Failure(1, 1, 1, 1, 1, 1);                       // System failure, Battery failure, Motor_fault, Motor_overheating, Auxiliary battery, Insulation failure
    set_E650_Dashboard_Tire_pressure(1, 1, 1, 1);                       // Tire_pressure (Wheel1, Wheel2, Wheel3, Wheel4)
    send_E650_Dashboard();
  }
}
