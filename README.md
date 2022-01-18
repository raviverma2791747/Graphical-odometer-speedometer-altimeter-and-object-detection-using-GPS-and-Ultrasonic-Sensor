# -Graphical-odometer-speedometer-altimeter-and-object-detection-using-GPS-and-Ultrasonic-Sensor
We commute daily in our vehicles to workplaces, schools, colleges, or homes. For example, when we go to our workplaces we observe some quantities like the speed of some moving object, many times are curious to know the location, sometimes when we are standing somewhere we want to know the altitude.So we just started about this idea and made a project to know location, altitude, the distance between two points, and speed.
We made a device which used the existing GPS technology to get the data we are interested in. There are various solutions to this problem like an android app. But as android devices are dedicated for personal use.So we made a device dedicated to the purpose.We used the touchscreen to display the speed, distance, altitude, and the location and ease the use of device, we added ability to record all the data collected in the device using an sd card and stored the data in .csv files which can be directly opened in Microsoft Excel or Python Scripts to process it and use to for any purpose. 

### Components Used
Arduino Mega 2560 microcontroller board
2.4 Inch Touch Screen TFT Display Shield
Ublox Neo-6M GPS Module
Ultrasonic Distance Sensor Module - HC-SR04
Micro SD Card Reader Module

Component Connections

GPS

| Sensor Pins | Arduino Mega 2560 Pins |
| --- | --- |
| GND | GND |
| VCC | 5V VCC |
| RX  | digital pin 18(TX1) |
| TX  | digital pin 19(RX1) |

Ultrasonic

| Sensor Pins | Arduino Mega 2560 Pins |
| --- | --- |
| GND | GND |
| VCC | 5V VCC |
| Trig | digital pin 44(PWM) |
| Echo | digital pin 45(PWM) |

MicroSD

| Sensor Pins | Arduino Mega 2560 Pins |
| --- | --- |
| GND | GND |
| VCC | 5V VCC |
| CS  | digital pin 53(SS) |
| SCK | digital pin 52(SCK) |
| MOSI | digital pin 51(MOSI) |
| MISO | digital pin 50(MISO) |

2.4 Inch TFT LCD Touch shield

| Sensor Pins | Arduino Mega 2560 Pins |
| --- | --- |
| GND | GND |
| 3.3V | 3.3V VCC |
| 5V  | 5V VCC |
| LCD_RD | analog pin 0 |
| LCD_WR | analog pin 1 |
| LCD_RS | analog pin 2 |
| LCD_CS | analog pin 3 |
| LCD_RST | analog pin 4 |
| F_CS | analog pin 5 |
| LCD_D0 | digital pin 8 |
| LCD_D1 | digital pin 9 |
| LCD_D2 | digital pin 2 |
| LCD_D3 | digital pin 3 |
| LCD_D4 | digital pin 4 |
| LCD_D5 | digital pin 5 |
| LCD_D6 | digital pin 6 |
| LCD_D7 | digital pin 7 |
| SD_SCK | digital pin 13 |
| SD_D0 | digital pin 12 |
| SD_D1 | digital pin 11 |
| SD_SS | digital pin 10 |
