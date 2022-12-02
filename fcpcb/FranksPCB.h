// FranksPCB.h
// Runs on TM4C123
// Includes all PCB drivers in one file
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22

// you can include this header file to include all pcb headers
// but you still must add all .cpp files to source
// header also establishes global objects for each element of pcb
// Joystick, Slidepot, Dpad, Launchpad buttons, and Dac
// 

#ifndef FRANKSPCB_H
#define FRANKSPCB_H

#include "JoyStick.h"
#include "SlidePot.h"
#include "DPad.h"
#include "LaunchPad.h"
#include "Sound.h"

SlidePot slide(0);
JoyStick joy(128);
DPad dpad;
LaunchPad lpad;
DAC dac;

#endif
