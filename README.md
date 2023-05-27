# ECE-319K-game-competition
![Game PCB](/PCB.PNG)
For Intro to Embedded Systems at the University of Texas at Austin, I created a handheld game with the help of my lab parter Jasmine Pok that won 2nd place in the in-class competition. The game runs on a PCB I designed on a 16-bit color 128x160 LCD. It has 6-bit sound and runs smoothly at 30Hz. The game is a UT-themed rougelike inspired dungeon crawler, where the player advances through 5 levels with increasing difficulty, unique and scaling enemies on each level, and a boss fight at the end in a quest to rescue Bevo, the UT longhorn. The player collects coins along the way which they can use for a variety of different upgrade options in preperation for the boss fight.
### /Lab10_C++
Contains main file and other files closely related to this project specifically. All game engine objects & logic are defined directly in the main.cpp file, this was done for speed, rapid prototyping & changing, and experimentation, and to keep everything in-lined. I will break them into separate files should I re-use them.  
### /fcpcb
Contains all the drivers I wrote for the PCB I designed for this project. The PCB is designed to connect to a TM4C123G Launchpad via TI's booster pack connectors and contains a 128x160 TFT LCD display, Joystick, 4 more buttons, 6 LEDs, a 6-bit DAC with a headphone jack and speaker, a slidepot, & a serial port. Note that tm4c123gh6pm.h and ST7735.h & .cpp were NOT written by me, the former provided by Texas Instruments and the latter originally by the LCD manufacturer, modified by my professors as specified in the file, and tweaked by me slightly. Used with permission.
### /flib
Contains all of the misc utility libraries I wrote for various reasons, as no standard libraries other than stdint were allowed. Also includes software to drive the TM4C's hardware timers, and a WIP linear algebra library that I didn't end up using but will probably create a dedicated repo for soon.
### /inc
Contains the provided ECE 319K files that were used in this project. The only one used was a set of assembly decimal & fixed point to string functions, which I wrote the definitions for for a previous lab & reused here.
