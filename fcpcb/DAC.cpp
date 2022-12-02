// DAC.cpp
// Runs on TM4C123
// DAC function definitions
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include "DAC.h"


// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x0000002;
  delay = 666;
	GPIO_PORTB_DEN_R |= 0x0000003F;
	GPIO_PORTB_DIR_R |= 0x0000003F;
	GPIO_PORTB_DR8R_R |= 0x0000003F;
	GPIO_PORTB_PCTL_R &= ~(0x00FFFFFF);

}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint8_t data){
	GPIO_PORTB_DATA_R = data;
}
