/*************************************************************************************************************************
 CAN protocol: Motorola format, baud rate: 500kbps
 A3:  The seat belt is not fastened with the indication (12V-)
 A8:  CAN-H 
 A9:  CAN-L
 A10: GND
 A11: BATT(12V+)
 A12: TGN (12V+)
 A20: Braking fault indication (12V-)
 ________________________________________________________________________________________________________________________
 
 Gauge Type
 -  0x226 Speed
 -  0x101 RPM
 -  0x251 Fuel gauge
 -  0x251 Voltage gauge
 -  0x251 Current meter
 -  0x390 Range
 -  0x101 Power

 Light Type (- Dial , * LCD screen)
 -  0x302 Right turn
 -  0x302 Left turn
 *  0x3C0 Run preparation (Ready)
 -  0x390 System failure
 *  0x3C3 Power battery heating indication
 -  0x3C3 Power battery failure
 -  0x3C3 Charging cable
 *  0x302 Door alarm indications
 -  (A3*Low level)  Seat belt indication
 -  0x302 rear fog lights
 -  0x302 High beam
 *  0x228, 0x345  Tire pressure monitoring status
 *  (*SOC <20%) Low battery indicator
 -  0x302 Position lights
 *  0x112 Motor overheating indication
 -  (*Low level) Brake fault indication
 -  0x3C0 Park brake
 -  0x3C6 EPS indications
 -  0x3C0 Power reduction
 -  0x3C3 Battery cut-off
 *  0x226 ABS indication
 -  0x112 motor fault indication
 -  0x3C3 Charge status indicatoin
 -  (No data) Airbag indications
 -  0x3C3 Indication of insulation failure
 -  0x5B3 DCDC indication 
 -  0x3C0 rd gear (S, D, N, R)
  ________________________________________________________________________________________________________________________
 
 Meter data input (cycle <1500 ms)
 RPM                          : 0x101 -> Byte1:LowByte, Byte0:HighByte (Offset -15000) (0 to 9900rpm) Unit:rpm
 Power                        : 0x101 -> Byte5 (0 to 100%) Unit:%POWER

 Motor overheating indication : 0x112 -> Byte6, Bit3
 motor fault indication       : 0x112 -> Byte6, Bit4

 Voltage gauge                : 0x251 -> Byte1:LowByte, Byte0:HighByte (precision 0.01) (0 to 500V) Unit:V
 Current meter                : 0x251 -> Byte3:LowByte, Byte2:HighByte (precision 0.1, Offset -500) (-500 to 500A) Unit:A
 Fuel gauge                   : 0x251 -> Byte4  Unit:% (<20%, <40%, <60%, <80%, 80% upper)

 ABS indication               : 0x226 -> Byte0, Bit1
 Speed                        : 0x226 -> Byte2:LowByte, Byte1:HighByte (precision 0.05625) (0 to 199km/h) Unit:km/h

 Right turn                   : 0x302 -> Byte0, Bit4
 Left turn                    : 0x302 -> Byte0, Bit5
 rear fog lights              : 0x302 -> Byte0, Bit7
 Door alarm indications       : 0x302 -> Byte1, Bit(4, 5, 6, 7)
 Position lights              : 0x302 -> Byte5, Bit5
 High beam                    : 0x302 -> Byte5, Bit7

 System failure               : 0x390 -> Byte3, Bit6
 Range                        : 0x390 -> Byte5:LowByte, Byte4:HighByte  (0 to 500km) Unit:km

 Run preparation (Ready)      : 0x3C0 -> Byte0, Bit0
 Power reduction              : 0x3C0 -> Byte1, Bit0
 Park brake                   : 0x3C0 -> Byte5, Bit7
 rd gear                      : 0x3C0 -> Byte6, (0x00:R, 0x01:N, 0x02:D, 0x03:S)

 Charge status indicatoin           : 0x3C3 -> Byte3, (0x10:Flashing, 0x20:Solid on)
 Charging cable                     : 0x3C3 -> Byte5, Bit0
 Power battery failure              : 0x3C3 -> Byte5, Bit1
 Indication of insulation failure   : 0x3C3 -> Byte5, Bit2
 Battery cut-off                    : 0x3C3 -> Byte5, Bit3
 Power battery heating indication   : 0x3C3 -> Byte6, Bit0

 EPS indications                    : 0x3C6 -> Byte0, Bit0

 Auxiliary battery charge indication: 0x5B3 -> Byte5, (0x01 or 0x02 or 0x03 or 0x04 or 0x05:Light up) (DCDC_Fault)
 DCDC indication                    : 0x5B3 -> Byte5, (0x01 or 0x02 or 0x03 or 0x04 or 0x05 Light up)

 Tire pressure monitoring status    : 0x228 -> Byte0, Bit(2, 3)
                                    : 0x345 -> Byte0, Bit(0, 3)
 ________________________________________________________________________________________________________________________
 
 Meter data output (cycle 1000 ms)
 Accrued miles    : 0x623 -> Byte2:LowByte, Byte1, Byte0:HighByte Unit:km
 safety belt      : 0x623 -> Byte4, Bit6(0:Close, 1:Open)
 Brake failure    : 0x623 -> Byte4, Bit7(0:Normal, 1:Fault)

 Product code     : 0x624 -> Byte3:LowByte, Byte2, Byte1, Byte0:HighByte
 Hardware version : 0x624 -> Byte5:LowByte, Byte4:HighByte
 Software version : 0x624 -> Byte7:LowByte, Byte6:HighByte
 *************************************************************************************************************************/

uint8_t dataBUF1[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x101
uint8_t dataBUF2[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x112
uint8_t dataBUF3[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x251
uint8_t dataBUF4[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x226
uint8_t dataBUF5[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x302
uint8_t dataBUF6[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x3C0
uint8_t dataBUF7[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x3C3
uint8_t dataBUF8[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x3C6
uint8_t dataBUF9[8]   = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x5B3
uint8_t dataBUF10[8]  = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x228
uint8_t dataBUF11[8]  = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x345
uint8_t dataBUF12[8]  = {0, 0, 0, 0, 0, 0, 0, 0}; //ID:0x390


// Speed(0 to 199km/h), RPM(0 to 9900rpm), Fuel(0 to 100%), Voltage(0 to 500V), Current(-500 to 500A), Range(0 to 500km), Power(0 to 100%)
void set_E650_Dashboard_Gauge(float Speed, uint16_t RPM, uint8_t Fuel, float Voltage, float Current, uint16_t Range, uint8_t Power)
{
  // limit parameter
  if(Speed<=0){Speed=0;}if(Speed>=199){Speed=199;}
  if(RPM<=0){RPM=0;}if(RPM>=9900){RPM=9900;}
  if(Fuel<=0){Fuel=0;}if(Fuel>=100){Fuel=100;}
  if(Voltage<=0){Voltage=0;}if(Voltage>=500){Voltage=500;}
  if(Current<=-500){Current=-500;}if(Current>=500){Current=500;}
  if(Range<=0){Range=0;}if(Range>=500){Range=500;}
  if(Power<=0){Power=0;}if(Power>=100){Power=100;}
  
  // calculated offset and resolution
  uint16_t pSpeed = Speed/0.05625;
  RPM = RPM+15000;
  uint16_t pVoltage = Voltage/0.1;
  uint16_t pCurrent = (Current+500)/0.1;

  // Add RPM to 0x101
  dataBUF1[0] = RPM >> 8;
  dataBUF1[1] = RPM & 0xFF;
  // Add Power to 0x101
  dataBUF1[5] = Power;
  
  // Add Voltage to 0x251
  dataBUF3[0] = pVoltage >> 8;
  dataBUF3[1] = pVoltage &0xFF;
  // Add Current to 0x251
  dataBUF3[2] = pCurrent >> 8;
  dataBUF3[3] = pCurrent &0xFF;
  dataBUF3[4] = Fuel;
  
  // Add Speed to 0x226
  dataBUF4[1] = pSpeed >> 8;
  dataBUF4[2] = pSpeed &0xFF;
  
  
  // Add Range to 0x390
  dataBUF12[4] = Range >> 8;
  dataBUF12[5] = Range; 
}


// Gear('R', 'N', 'D', 'S'), Gear_effective_position, Run_Preparation
void set_E650_Dashboard_Gear(char rd_gear, byte Gear_effective_position, byte Run_Preparation)
{
  switch (rd_gear)
  {
    case 'R':
            dataBUF6[6] = 0x00;
            break;
    case 'N':
            dataBUF6[6] = 0x01;           
            break;
    case 'D':
            dataBUF6[6] = 0x02;
            break;
    case 'S':
            dataBUF6[6] = 0x03;
            break;     
  }
  dataBUF6[0] = (Gear_effective_position << 4) | Run_Preparation;
}


// turnLeft, turnRight
void set_E650_Dashboard_TurnSignal(byte turnLeft, byte turnRight)
{
  switch (turnLeft)
  {
    case 0:
            dataBUF5[0] &= 0x90;
            break;
    case 1:
            (dataBUF5[0] &= 0x90) |= 0x20;
            break;
  }
  switch (turnRight)
  {
    case 0:
            dataBUF5[0] &= 0xA0;
            break;
    case 1:
            (dataBUF5[0] &= 0xA0) |= 0x10;
            break;
  }
}


// position_light, high_beam, rear_fog
void set_E650_Dashboard_Light(byte position_light, byte high_beam, byte rear_fog)
{
  switch (position_light)
  {
    case 0:
            dataBUF5[5] &= 0x80;
            break;
    case 1:
            (dataBUF5[5] &= 0x80) |= 0x20;
            break;
  }
  switch (high_beam)
  {
    case 0:
            dataBUF5[5] &= 0x20;
            break;
    case 1:
            (dataBUF5[5] &= 0x20) |= 0x80;
            break;
  }
  switch (rear_fog)
  {
    case 0:
            dataBUF5[0] &= 0x30;
            break;
    case 1:
            (dataBUF5[0] &= 0x30) |= 0x80;
            break;
  }
}


// Door1, Door2, Door3, Door4
void set_E650_Dashboard_Door_alarm(byte door1, byte door2, byte door3, byte door4)
{
  switch (door1)
  {
    case 0:
            dataBUF5[1] &= 0xE0;
            break;
    case 1:
            (dataBUF5[1] &= 0xE0) |= 0x10;
            break;
  }
  switch (door2)
  {
    case 0:
            dataBUF5[1] &= 0xD0;
            break;
    case 1:
            (dataBUF5[1] &= 0xD0) |= 0x20;
            break;
  }
  switch (door3)
  {
    case 0:
            dataBUF5[1] &= 0xB0;
            break;
    case 1:
            (dataBUF5[1] &= 0xB0) |= 0x40;
            break;
  }
  switch (door4)
  {
    case 0:
            dataBUF5[1] &= 0x70;
            break;
    case 1:
            (dataBUF5[1] &= 0x70) |= 0x80;
            break;
  }
}


// Park brake, ESP, Power reduction, Battery cut-off, ABS, Charging cable, Charge_status(0:Off, 1:Flashing, 2:Solid On)
void set_E650_Dashboard_Alarm(byte Park_brake, byte ESP, byte Power_reduction, byte Battery_cutoff, byte ABS, byte Charging_cable, uint8_t Charge_status)
{
  switch (Park_brake)
  {
    case 0:
            dataBUF6[5] &= 0x7F;
            break;
    case 1:
            (dataBUF6[5] &= 0x7F) |= 0x80;
            break;
  }
  switch (ESP)
  {
    case 0:
            dataBUF8[0] = 0x00;
            break;
    case 1:
            dataBUF8[0] = 0x01;
            break;
  }
  switch (Power_reduction)
  {
    case 0:
            dataBUF6[1] = 0x00;
            break;
    case 1:
            dataBUF6[1] = 0x01;
            break;
  }
  switch (Battery_cutoff)
  {
    case 0:
            dataBUF7[5] &= 0x07;
            break;
    case 1:
            (dataBUF7[5] &= 0x07) |= 0x08;
            break;
  }
  switch (ABS)
  {
    case 0:
            dataBUF4[0] = 0x00;
            break;
    case 1:
            dataBUF4[0] = 0x02;
            break;
  }
  switch (Charging_cable)
  {
    case 0:
            dataBUF7[5] &= 0x0E;
            break;
    case 1:
            (dataBUF7[5] &= 0x0E) |= 0x01;
            break;
  }
  switch (Charge_status)
  {
    case 0:
            dataBUF7[3] = 0x00; // Off
            break;
    case 1:
            dataBUF7[3] = 0x10; // Flashing
            break;
    case 2:
            dataBUF7[3] = 0x20; // Solid on
            break;
  }
}


// System failure, Battery failure, Motor fault, Motor_overheating, Auxiliary battery, Insulation failure
void set_E650_Dashboard_Failure(byte System_failure, byte Battery_failure ,byte motor_fault, byte motor_overheating, byte Auxiliary_battery, byte insulation_failure)
{
  switch (System_failure)
  {
    case 0:
            dataBUF12[3] = 0x00;
            break;
    case 1:
            dataBUF12[3] = 0x40;
            break;
  }
  switch (Battery_failure)
  {
    case 0:
            dataBUF7[5] &= 0x0D;
            break;
    case 1:
            (dataBUF7[5] &= 0x0D) |= 0x02;
            break;
  }
  switch (motor_fault)
  {
    case 0:
            dataBUF2[6] &= 0x08;
            break;
    case 1:
            (dataBUF2[6] &= 0x08) |= 0x10;
            break;
  }
  switch (motor_overheating)
  {
    case 0:
            dataBUF2[6] &= 0x10;
            break;
    case 1:
            (dataBUF2[6] &= 0x10) |= 0x08;
            break;
  }
  switch (Auxiliary_battery)
  {
    case 0:
            dataBUF9[5] = 0x00;
            break;
    case 1:
            dataBUF9[5] = 0x07;
            break;
  }
  switch (insulation_failure)
  {
    case 0:
            dataBUF7[5] &= 0x0B;
            break;
    case 1:
            (dataBUF7[5] &= 0x0B) |= 0x04;
            break;
  }
}


void set_E650_Dashboard_Tire_pressure(byte wheel1, byte wheel2, byte wheel3, byte wheel4)
{
  switch (wheel1)
  {
    case 0:
            dataBUF10[0] &= 0x08;
            break;
    case 1:
            (dataBUF10[0] &= 0x08) |= 0x04;
            break;
  }
  switch (wheel2)
  {
    case 0:
            dataBUF10[0] &= 0x04;
            break;
    case 1:
            (dataBUF10[0] &= 0x04) |= 0x08;
            break;
  }
  switch (wheel3)
  {
    case 0:
            dataBUF11[0] &= 0x08;
            break;
    case 1:
            (dataBUF11[0] &= 0x08) |= 0x01;
            break;
  }
  switch (wheel4)
  {
    case 0:
            dataBUF11[0] &= 0x01;
            break;
    case 1:
            (dataBUF11[0] &= 0x01) |= 0x08;
            break;
  }
}


void send_E650_Dashboard()
{
  if((dataBUF1[0]!=0x00)||(dataBUF1[1]!=0x00)||(dataBUF1[2]!=0x00)||(dataBUF1[3]!=0x00)||(dataBUF1[4]!=0x00)||(dataBUF1[5]!=0x00)||(dataBUF1[6]!=0x00)||(dataBUF1[7]!=0x00))
  {
    CAN.sendMsgBuf(0x101, 0, 8, dataBUF1);
  }
  if((dataBUF2[0]!=0x00)||(dataBUF2[1]!=0x00)||(dataBUF2[2]!=0x00)||(dataBUF2[3]!=0x00)||(dataBUF2[4]!=0x00)||(dataBUF2[5]!=0x00)||(dataBUF2[6]!=0x00)||(dataBUF2[7]!=0x00))
  {
    CAN.sendMsgBuf(0x112, 0, 8, dataBUF2);
  }
  if((dataBUF3[0]!=0x00)||(dataBUF3[1]!=0x00)||(dataBUF3[2]!=0x00)||(dataBUF3[3]!=0x00)||(dataBUF3[4]!=0x00)||(dataBUF3[5]!=0x00)||(dataBUF3[6]!=0x00)||(dataBUF3[7]!=0x00))
  {
    CAN.sendMsgBuf(0x251, 0, 8, dataBUF3);
  }
  if((dataBUF4[0]!=0x00)||(dataBUF4[1]!=0x00)||(dataBUF4[2]!=0x00)||(dataBUF4[3]!=0x00)||(dataBUF4[4]!=0x00)||(dataBUF4[5]!=0x00)||(dataBUF4[6]!=0x00)||(dataBUF4[7]!=0x00))
  {
    CAN.sendMsgBuf(0x226, 0, 8, dataBUF4);
  }
  if((dataBUF5[0]!=0x00)||(dataBUF5[1]!=0x00)||(dataBUF5[2]!=0x00)||(dataBUF5[3]!=0x00)||(dataBUF5[4]!=0x00)||(dataBUF5[5]!=0x00)||(dataBUF5[6]!=0x00)||(dataBUF5[7]!=0x00))
  {
    CAN.sendMsgBuf(0x302, 0, 8, dataBUF5);
  }
  if((dataBUF6[0]!=0x00)||(dataBUF6[1]!=0x00)||(dataBUF6[2]!=0x00)||(dataBUF6[3]!=0x00)||(dataBUF6[4]!=0x00)||(dataBUF6[5]!=0x00)||(dataBUF6[6]!=0x00)||(dataBUF6[7]!=0x00))
  {
    CAN.sendMsgBuf(0x3C0, 0, 8, dataBUF6);
  }
  if((dataBUF7[0]!=0x00)||(dataBUF7[1]!=0x00)||(dataBUF7[2]!=0x00)||(dataBUF7[3]!=0x00)||(dataBUF7[4]!=0x00)||(dataBUF7[5]!=0x00)||(dataBUF7[6]!=0x00)||(dataBUF7[7]!=0x00))
  {
    CAN.sendMsgBuf(0x3C3, 0, 8, dataBUF7);
  }
 
  CAN.sendMsgBuf(0x3C6, 0, 8, dataBUF8);

  if((dataBUF9[0]!=0x00)||(dataBUF9[1]!=0x00)||(dataBUF9[2]!=0x00)||(dataBUF9[3]!=0x00)||(dataBUF9[4]!=0x00)||(dataBUF9[5]!=0x00)||(dataBUF9[6]!=0x00)||(dataBUF9[7]!=0x00))
  {
    CAN.sendMsgBuf(0x5B3, 0, 8, dataBUF9);
  }
  if((dataBUF10[0]!=0x00)||(dataBUF10[1]!=0x00)||(dataBUF10[2]!=0x00)||(dataBUF10[3]!=0x00)||(dataBUF10[4]!=0x00)||(dataBUF10[5]!=0x00)||(dataBUF10[6]!=0x00)||(dataBUF10[7]!=0x00))
  {
    CAN.sendMsgBuf(0x228, 0, 8, dataBUF10);
  }
  if((dataBUF11[0]!=0x00)||(dataBUF11[1]!=0x00)||(dataBUF11[2]!=0x00)||(dataBUF11[3]!=0x00)||(dataBUF11[4]!=0x00)||(dataBUF11[5]!=0x00)||(dataBUF11[6]!=0x00)||(dataBUF11[7]!=0x00))
  {
    CAN.sendMsgBuf(0x345, 0, 8, dataBUF11);
  }
  if((dataBUF12[0]!=0x00)||(dataBUF12[1]!=0x00)||(dataBUF12[2]!=0x00)||(dataBUF12[3]!=0x00)||(dataBUF12[4]!=0x00)||(dataBUF12[5]!=0x00)||(dataBUF12[6]!=0x00)||(dataBUF12[7]!=0x00))
  {
    CAN.sendMsgBuf(0x390, 0, 8, dataBUF12);
  }
}
