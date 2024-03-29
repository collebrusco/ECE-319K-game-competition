# ECE-319K-game-competition
![Game PCB](/PCB.PNG)    
## [Youtube demo](https://youtu.be/LUv89gF3i-0)

2nd place! 'Save Bevo' is a handheld game built with a custom PCB & TM4C microcontroller. The system includes an LCD, DAC, joystick, slide-pot, buttons, & LEDs. The game is a simple overhead 'bullet hell' style game with 5 levels, various enemy types and a currency and upgrade system.  

This game was created for ECE 319k at UT Austin. This repository contains all of the software that runs on the TM4C: a combination of 32-bit ARM assembly, C, and C++.

### /Lab10_C++
Contains main file and other files closely related to this project specifically. All game engine objects & logic are defined directly in the main.cpp file. This would have to change in a larger system, but for now it means you can read all of the game logic in one place.
### /fcpcb
Contains all the drivers I wrote for the PCB I designed for this project. The PCB is designed to connect to a TM4C123G Launchpad via TI's booster pack connectors and contains a 128x160 TFT LCD display, Joystick, 4 more buttons, 6 LEDs, a 6-bit DAC with a headphone jack and speaker, a slidepot, & a serial port. Note that tm4c123gh6pm.h and ST7735.h & .cpp were NOT written by me, the former provided by Texas Instruments and the latter originally by the LCD manufacturer, modified by my professors as specified in the file, and tweaked by me slightly. Used with permission.
### /flib
Contains all of the misc utility libraries I wrote for various reasons, as no standard libraries other than stdint were allowed. Also includes software to drive the TM4C's hardware timers, and a WIP linear algebra library that I didn't end up using but will probably create a dedicated repo for soon.
### /inc
Contains the provided ECE 319K files that were used in this project. The only one used was a set of assembly decimal & fixed point to string functions, which I wrote the definitions for for a previous lab & reused here.
