// DAC.h
// Runs on TM4C123
// DAC init & out functions
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22


#ifndef DAC_H
#define DAC_H
#include <stdint.h>
// Header files contain the prototypes for public functions
// this file explains what the module does

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint8_t data);

#endif
