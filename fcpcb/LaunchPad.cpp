#include "LaunchPad.h"
#include "tm4c123gh6pm.h"
// LaunchPad.cpp
// Runs on TM4C123
// LaunchPad Object Definitions
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22

#define L_BOARD (GPIO_PORTF_DATA_R & 0x01)
#define R_BOARD (GPIO_PORTF_DATA_R & 0x10)

#define PF3 (*((volatile uint32_t *)0x40025020))
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))

LaunchPad::LaunchPad(void){
    SYSCTL_RCGCGPIO_R |= 0x20;
    {volatile uint32_t delay = 666;}
		GPIO_PORTF_LOCK_R = 0x4C4F434B;
		GPIO_PORTF_CR_R |= 0x01;
    GPIO_PORTF_DEN_R |= 0x1F;
    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTF_DIR_R &= ~(0x11);
    GPIO_PORTF_PUR_R |= 0x11;
}

extern "C" long StartCritical(void);
extern "C" void EndCritical(long);
void LaunchPad::sample(void){
	lPrev = L;
	rPrev = R;
	//long sr = StartCritical();
	L = !L_BOARD;
	R = !R_BOARD;
	//EndCritical(sr);
}

bool LaunchPad::l(void){
	return L;
}
bool LaunchPad::r(void){
	return R;
}

bool LaunchPad::l_ris(void){
	return L && !lPrev;
}

bool LaunchPad::r_ris(void){
	return R && !rPrev;
}

