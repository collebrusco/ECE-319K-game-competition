#include "Dpad.h"
#include "tm4c123gh6pm.h"
// DPad.cpp
// Runs on TM4C123
// DPad Object Definitions
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22

//four bool values are stored in bottom 4 bits of integer data variable
// | BUTTON |   A   |   B   |   X   |   Y   |
// |  BIT   |   3   |   2   |   1   |   0   |
// |  PORT  |  PE0  |  PE1  |  PE3  |  PE2  |
#define AM 0x00000008
#define BM 0x00000004
#define XM 0x00000002
#define YM 0x00000001
#define AS 3
#define BS 1
#define XS 2
#define YS 2
#define A_BOARD (GPIO_PORTE_DATA_R & 0x01)
#define B_BOARD (GPIO_PORTE_DATA_R & 0x02)
#define X_BOARD (GPIO_PORTE_DATA_R & 0x08)
#define Y_BOARD (GPIO_PORTE_DATA_R & 0x04)

DPad::DPad(void){
    SYSCTL_RCGCGPIO_R |= 0x10;
    {volatile uint32_t delay = 666;}
    GPIO_PORTE_DEN_R |= 0x0F;
    GPIO_PORTE_DIR_R &= ~(0x0f);
}
extern "C" long StartCritical(void);
extern "C" void EndCritical(long);
void DPad::sample(void){
	aPrev = A;
	bPrev = B;
	xPrev = X;
	yPrev = Y;
	//long sr = StartCritical();
	A = A_BOARD;
	B = B_BOARD;
	X = X_BOARD;
	Y = Y_BOARD;
	//EndCritical(sr);
}

bool DPad::a(void){
	return A;
}
bool DPad::b(void){
	return B;
}
bool DPad::x(void){
	return X;
}
bool DPad::y(void){
	return Y;
}

bool DPad::a_ris(void){
	return A && !aPrev;
}

bool DPad::b_ris(void){
	return B && !bPrev;
}

bool DPad::x_ris(void){
	return X && !xPrev;
}

bool DPad::y_ris(void){
	return Y && !yPrev;
}
