//flib/SysTick.h
//Enable/Disable Timers 0A - 4A
//Frank Collebrusco 11/2022
//tm4c123gh6pm
#include <stdint.h>


void SysTick_Init(void (*task)(void), uint32_t period, uint32_t pri);


void SysTick_Stop(void);


