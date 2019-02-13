# IRLock

## Introduction
This project consists in a digital lock, that takes input from an IR remote control and compare passwords, if the password checks, the lock opens or closes, there are a buzzer for sounds, LCD for visualization and a servo for simulating the "action".
	
---

## Materials
The materials used are:
* Arduino Uno
* LCD - 1602
* IR Sensor - VS1838B
* Remote Control
* Buzzer
* Potentiometer
* Servo - SG90
* Jumpers
* Breadboard

---

## Circuit
Consult the datasheet of your components to get the right pinout.  

The Fritzing file with the schematic can be found in the /doc folder.

![alt text](https://github.com/gustavoaguilar/IRLock/blob/master/doc/IRLock_Sketch_bb.png "Circuit")

---

## Code
IRLock was written with the Visual Studio Code PlatformIO extension, but the code was ported(removed the **#include<Arduino.h>** line) to Arduino IDE for more convenience.

To make the prototyping faster, I used libraries, but it came with a drawback, I lost the control of the timers from the microcontroller, because of that, lost the possibility to use the "tone" function, so I got the *myTone()* code from the user @jurs  at this [arduino forum topic](https://forum.arduino.cc/index.php?topic=339625.0).

### Dependencies
These are the needed libraries:

* LyquidCrystal
* IRremote

### Some Highlights
This first section is where you may change the pins used according to your circuit:

```c
#define RECV_PIN 7
#define SERVO_PIN 9
#define BUZZER_PIN 6

#define LCD_RS_PIN 12
#define LCD_ENABLE_PIN 11
#define LCD_D4_PIN 5
#define LCD_D5_PIN 4
#define LCD_D6_PIN 3
#define LCD_D7_PIN 2
```

**This next line sets the IR codes in a vector, you must change this values to the ones your remote control produces.**  

```c
long unsigned int keys[13] {0x207F08F7, 0x207F8877, 0x207F48B7, 0x207FC837, 0x207F28D7, 0x207FA857, 0x207F6897, 0x207FE817, 0x207F18E7, 0x207F9867, 0x207F38C7, 0x207FF807, 0x207F906F};
```


To make life easier, these are the positions of the vector:

* [0] to [9] are the **numbers** buttons (0 to 9)
* [10] is the **Erase** button
* [11] is the **Enter** button
* [12] is the **New Password** button

---

## Issues

1. Sometimes the system detects the IR signal, but can't decode it correctly. (probably a remote control problem)

---

## Contribute
If you have any suggestion to improve this project, let me know. :)
