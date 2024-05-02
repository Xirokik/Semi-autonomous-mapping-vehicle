# Semi-autonomous-mapping-vehicle

## Descriptions
The vehicle making a map of the area using an ultrasonic distance sensor.

The values of the motor control signals, forward driving time and turning times were determined based on experience and may vary depending on the platform used, motors and supply voltage values.

The vehicle performs a scan, then moves forward by 10 cm and performs another scan. If it encounters an obstacle in front of itself, the vehicle will check in which direction there is a greater distance to the next obstacle and turn in that direction

The creating map is saved in .txt format on the microSD card:
- a single measurement is saved as "11" to maintain approximate distance scaling,
- one line corresponds to 1 cm, two characters in a line correspond to 1 cm,
- the x symbol indicates the place where the measurement was taken.

## Functionality

After establishing connection with the microSD card, an empty map with the specified dimensions will be created. After completing it, it will be possible to activate the mapping process:
- button A activates the process and opens the map file,
- button B closes the edited map file and ends the process.

WARNING

THE MAP FILE MUST BE CLOSE TO SAVE THE CHANGES!

## Libraries
```
#include <SPI.h>
#include <SD.h>
#include <Servo.h>
```

## Components
1) Arduino Uno rev 3
2) Tank chassis (I used Black gladiator chassis, 6-12V motors included)
3) Two-channel motor controller L298N
4) Ultrasonic distance sensor HC-SR04 
5) Servo TowerPro SG-90 - micro - 180 degrees
6) Mounting bracket for distance sensor HC-SR04
7) Micor SD Card
8) Micro SD Card Reader Module
9) 9v battery with Adapter plug DC 5.5 / 2.1 for Arduino
10) 6x 1,5V (AA batteries) for motors
11) Battery basket 6x1,5V (AA batteries)
12) Bunch of cables
13) Four-channel 433 MHz radio module + remote control
14) Tape or glue for assembly
15) Breadboard (optional)

### Watch project in action [here](https://youtu.be/t9z7wJDY7MY)

### Mapped area
![](https://github.com/Xirokik/Semi-autonomous-mapping-vehicle/blob/main/Project's_files/Mapped_area.jpg)

### Area map
![](https://github.com/Xirokik/Semi-autonomous-mapping-vehicle/blob/main/Project's_files/Area_map.png)
