//flib/Timers.h
//Enable/Disable Timers 0A - 4A
//Frank Collebrusco 11/2022
//tm4c123gh6pm
#include <stdint.h>

//********TimerxA_Init********
//input: Interrupt period, units of 1/80MHz
//no output
//will initialize and enable periodic interrupts
//with given timer & max priority.
//User writes void TIMERxA_Handler(void) 
void TIMER0A_Init(void (*task)(void), uint32_t period, uint32_t pri);
void TIMER1A_Init(void (*task)(void), uint32_t period, uint32_t pri);
void TIMER2A_Init(void (*task)(void), uint32_t period, uint32_t pri);
void TIMER3A_Init(void (*task)(void), uint32_t period, uint32_t pri);
void TIMER4A_Init(void (*task)(void), uint32_t period, uint32_t pri);

//********TimerxA_Stop********
//no in/out
//Stops given timer from interrupting.
//Call init again to re-enable
void TIMER0A_Stop(void);
void TIMER1A_Stop(void);
void TIMER2A_Stop(void);
void TIMER3A_Stop(void);
void TIMER4A_Stop(void);

