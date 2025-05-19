# **VCU_BHL Board Communication Protocol**
* Pinout (ESP32)
    * 02 : LED Built-in
    * 05 : CS (SPI) 
    * 25 : Digital Input1
    * 25 : Digital Input2
    * 27 : Relay1
    * 04 : Relay2
    * 22 : Buzzer
    * 32 : Analog Input1
    * 33 : Analog Input2
    * 34 : Acceletor Input
    * 13 : PWM1
    * 14 : PWM2
* baud rate  500kbps
* Intel format
* Extended
### *data input*

* ID: 0x1500A150

|Byte|Description
|:-:|:-|
| 0 | BIT-4 : Relay1 , BIT-0 : Relay2|
| 1 | error code|
| 2 | Freq PWM1 (LOW  Byte)|
| 3 | Freq PWM1 HIGH Byte|
| 4 | Duty PWM1|
| 5 | Freq PWM2 LOW  Byte|
| 6 | Freq PWM2 HIGH Byte|
| 7 | Duty PWM2|

#
### *data output*
* ID: 0x150050A1

|Byte|Description|
|:-:|:-|
| 0 | BIT-4 : Relay1 , BIT-0 : Relay2|
| 1 | error code|
| 2 | Freq PWM1 (LOW  Byte)|
| 3 | Freq PWM1 HIGH Byte|
| 4 | Duty PWM1|
| 5 | Freq PWM2 LOW  Byte|
| 6 | Freq PWM2 HIGH Byte|
| 7 | Duty PWM2|

#
* ID: 0x150050A2

|Byte|Description|
|:-:|:-|
| 0 | BIT-4 : D_INPUT1 , BIT-0 : D_INPUT2|
| 1 | error code|
| 2 | Volt Acceletor (LOW  Byte)|
| 3 | Volt Acceletor (HIGH  Byte)|
| 4 | Volt AnalogInput1 (LOW  Byte)|
| 5 | Volt AnalogInput1 (HIGH  Byte)|
| 6 | Volt AnalogInput2 (LOW  Byte)|
| 7 | Volt AnalogInput2 (HIGH  Byte)|

## Function
 * control Relay
 ```js
Relay(channel, state);
 ```
 * Read Voltage
 ```js
 readVolt(analogPin);
 ```
 * PWM
 ```js
 pwm(channel, frequency, duty_cycle);
 ```
 * Error code
 ```js
 error_code = 0;
 ```
#
#
#
# **E650 Dashboard Communication Protocol**
* Pinout
    * A03 :  The seat belt is not fastened with the indication (12V-)
    * A08 :  CAN-H 
    * A09 :  CAN-L
    * A10 : GND
    * A11 : BATT(12V+)
    * A12 : TGN (12V+)
    * A20 : Braking fault indication (12V-)
* baud rate  500kbps
* Motorola format
* Standard
### *data input*
* ID: 0x101

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 0 | RPM (HIGH Byte)|-15,000|1|rpm|
| 1 | RPM (LOW Byte)| | | |
| 5 | Power| | |%POWER|

* ID: 0x112

|Byte|Description|
|:-:|:-|
| 6 | [BIT-3] --> Motor overheating indication|
| 6 | [BIT-4] --> Motor fault indication|

* ID: 0x251

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 0 | Voltage gauge (HIGH Byte)| |0.01|V|
| 1 | Voltage gauge (LOW Byte)| | | |
| 2 | Current meter (HIGH Byte)|-500|0.1|A|
| 3 | Current meter (LOW Byte)| | | |
| 4 | Fuel gauge| | |%|

* ID: 0x226

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 0 | [BIT-1] --> ABS indication| | | |
| 1 | Speed  (HIGH Byte)| |0.05625|km/h|
| 2 | Speed  (LOW Byte)| | | |

* ID: 0x302

|Byte|Description|
|:-:|:-|
| 0 | [BIT-4] ----------> Right turn|
| 0 | [BIT-5] ----------> Left turn|
| 0 | [BIT-7] ----------> Rear fog lights|
| 1 | [BIT-4, 5, 6, 7] --> Door alarm indications|
| 5 | [BIT-5] ----------> Position lights|
| 5 | [BIT-7] ----------> High beam|

* ID: 0x390

|Byte|Description|Offset|resolution|Unit|
|:-:|:-|:-:|:-:|:-:|
| 3 | [BIT-6] --> System failure| | | |
| 4 | Range  (HIGH Byte)| | |km|
| 5 | Range  (LOW Byte)| | | |

* ID: 0x3C0

|Byte|Description|
|:-:|:-|
| 0 | [BIT-0] --> Run preparation (Ready)|
| 1 | [BIT-0] --> Power reduction |
| 5 | [BIT-7] --> Park brake|
| 6 | rd gear --> (0x00:R, 0x01:N, 0x02:D, 0x03:S)|

* ID: 0x3C3

|Byte|Description|
|:-:|:-|
| 3 |Charge status indicatoin --> ((0x10:Flashing, 0x20:Solid on)|
| 5 | [BIT-0] -------------------> Charging cable |
| 5 | [BIT-1] -------------------> Power battery failure|
| 5 | [BIT-2] -------------------> Indication of insulation failure|
| 5 | [BIT-3] -------------------> Battery cut-off |
| 6 | [BIT-0] -------------------> Power battery heating indication|

* ID: 0x3C6

|Byte|Description|
|:-:|:-|
| 0 | [BIT-0] --> EPS indications|

* ID: 0x5B3

|Byte|Description|
|:-:|:-|
| 5 |Auxiliary battery charge indication (DCDC_Fault)--> 0x07:Light up|

* ID: 0x228

|Byte|Description|
|:-:|:-|
| 0 |[BIT-2] --> Tire pressure monitoring status 1|
| 0 |[BIT-3] --> Tire pressure monitoring status 2|

* ID: 0x345

|Byte|Description|
|:-:|:-|
| 0 |[BIT-0] --> Tire pressure monitoring status 3|
| 0 |[BIT-3] --> Tire pressure monitoring status 4|

### *data input (Cycle 1000 ms)*
* ID: 0x623

|Byte|Description|
|:-:|:-|
| 0 |Accrued miles (HIGH Byte)|
| 1 |Accrued miles .......^..........|
| 2 |Accrued miles (LOW Byte)|
| 4 |[BIT-6] --> safety belt (0:Close, 1:Open)|
| 4 |[BIT-7] --> Brake failure (0:Normal, 1:Fault)|

* ID: 0x624

|Byte|Description|
|:-:|:-|
| 0 |Product code (HIGH Byte)|
| 1 |Product code .......^..........|
| 2 |Product code .......^..........|
| 3 |Product code (LOW Byte)|
| 4 |Hardware version (HIGH Byte)|
| 5 |Hardware version (LOW Byte)|
| 6 |Software version (HIGH Byte)|
| 7 |Software version (LOW Byte)|

## Function
* Gauge
 ```js
set_E650_Dashboard_Gauge(Speed, RPM, Fuel, Voltage, Current, Range, Power);
 ```
#
* Gear
 ```js
set_E650_Dashboard_Gear(Gear('R', 'N', 'D', 'S'), Gear_effective_position, Run_Preparation);
 ```
#
* Turn Signal
 ```js
set_E650_Dashboard_TurnSignal(turnLeft, turnRight);
 ```
#
* Light
 ```js
set_E650_Dashboard_Light(position_light, high_beam, rear_fog);
 ```
#
* Door alarm
 ```js
set_E650_Dashboard_Door_alarm(Door1, Door2, Door3, Door4);
 ```
#
* Alarm
 ```js
set_E650_Dashboard_Alarm(Park brake, ESP, Power reduction, Battery cut-off, ABS, Charging cable, Charge_status(0:Off, 1:Flashing, 2:Solid On));
 ```
#
* Failure
 ```js
set_E650_Dashboard_Failure(System failure, Battery failure, Motor_fault, Motor_overheating, Auxiliary battery, Insulation failure);
 ```
#
* Tire_pressure
 ```js
set_E650_Dashboard_Tire_pressure(Wheel1, Wheel2, Wheel3, Wheel4);
 ```
#
* Send data
 ```js
send_E650_Dashboard();
 ```

## Original source

​
This library is possible thanks to original [Longan Labs CAN FD Library](https://github.com/Longan-Labs/Longan_CANFD) library. Thank you, Longan Labs.

## License

[MIT](https://choosealicense.com/licenses/mit/)

