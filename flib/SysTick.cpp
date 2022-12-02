//flib/Timers.cpp
//Frank Collebrusco 11/22
//tm4c123gh6pm
#include "SysTick.h"
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
extern "C" long StartCritical(void);
extern "C" void EndCritical(long sr);

extern "C" void SysTick_Handler(void);

static void (*Task)(void);

static bool Clock = false;

// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(void(*task)(void), uint32_t period, uint32_t pri){
	if (pri > 7){pri = 7;}
	long sr = StartCritical();
	Task = task;
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = period;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R &= 0x00FFFFFF;
	NVIC_SYS_PRI3_R |= (pri<<29);
	NVIC_ST_CTRL_R = 0x07;
	Clock = true;
	EndCritical(sr);
}

void SysTick_Stop(void){
	if (Clock){
		NVIC_ST_CTRL_R = 0;
		Clock = false;
	}
}

void SysTick_Handler(void){
	Task();
}


