//flib/Timers.cpp
//Frank Collebrusco 11/22
//tm4c123gh6pm
#include "Timers.h"
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
extern "C" long StartCritical(void);
extern "C" void EndCritical(long sr);

//********TIMERxA_Handler********
//no in/out
//These the C interrupt functions that call your task.
//These are called by the NVIC every period, you don't call them! or else!
extern "C" void TIMER0A_Handler(void);
extern "C" void TIMER1A_Handler(void);
extern "C" void TIMER2A_Handler(void);
extern "C" void TIMER3A_Handler(void);
extern "C" void TIMER4A_Handler(void);

void (*Task0)(void);
void (*Task1)(void);
void (*Task2)(void);
void (*Task3)(void);
void (*Task4)(void);

bool Clock0 = false;
bool Clock1 = false;
bool Clock2 = false;
bool Clock3 = false;
bool Clock4 = false;

void TIMER0A_Init(void (*task)(void), uint32_t period, uint32_t pri){
	if (pri > 7){pri=0;}
	Task0 = task;
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x01; //activate T0
	{volatile int i = 0;}
	Clock0 = true;
	TIMER0_CTL_R = 0;
	TIMER0_CFG_R = 0;
	TIMER0_TAMR_R = 0x00000002;
	TIMER0_TAPR_R = 0;
	TIMER0_TAILR_R = period;
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	TIMER0_IMR_R = 0x00000001;
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF) | (pri<<29);
	NVIC_EN0_R |= (1<<19);
	TIMER0_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void TIMER1A_Init(void (*task)(void), uint32_t period, uint32_t pri){
	if (pri > 7){pri=0;}
	Task1 = task;
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x02; //activate T1
	Clock1 = true;
	{volatile int i = 0;}
	TIMER1_CTL_R = 0;
	TIMER1_CFG_R = 0;
	TIMER1_TAMR_R = 0x00000002;
	TIMER1_TAPR_R = 0;
	TIMER1_TAILR_R = period;
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	TIMER1_IMR_R = 0x00000001;
	NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF) | (pri<<13);
	NVIC_EN0_R |= (1<<21);
	TIMER1_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void TIMER2A_Init(void (*task)(void), uint32_t period, uint32_t pri){
	if (pri > 7){pri=0;}
	Task2 = task;
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x04; //activate T2
	Clock2 = true;
	{volatile int i = 0;}
	TIMER2_CTL_R = 0;
	TIMER2_CFG_R = 0;
	TIMER2_TAMR_R = 0x00000002;
	TIMER2_TAPR_R = 0;
	TIMER2_TAILR_R = period;
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;
	TIMER2_IMR_R = 0x00000001;
	NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF) | (pri<<29);
	NVIC_EN0_R |= (1<<23);
	TIMER2_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void TIMER3A_Init(void (*task)(void), uint32_t period, uint32_t pri){
	if (pri > 7){pri=0;}
	Task3 = task;
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x08; //activate T3
	{volatile int i = 0;}
	Clock3 = true;
	TIMER3_CTL_R = 0;
	TIMER3_CFG_R = 0;
	TIMER3_TAMR_R = 0x00000002;
	TIMER3_TAPR_R = 0;
	TIMER3_TAILR_R = period;
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;
	TIMER3_IMR_R = 0x00000001;
	NVIC_PRI5_R = (NVIC_PRI8_R&0x00FFFFFF) | (pri<<29);
	NVIC_EN1_R |= (1<<(35-32));
	TIMER3_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void TIMER4A_Init(void (*task)(void), uint32_t period, uint32_t pri){
	if (pri > 7){pri=0;}
	Task4 = task;
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x10; //activate T4
	{volatile int i = 0;}
	Clock4 = true;
	TIMER4_CTL_R = 0;
	TIMER4_CFG_R = 0;
	TIMER4_TAMR_R = 0x00000002;
	TIMER4_TAPR_R = 0;
	TIMER4_TAILR_R = period;
	TIMER4_ICR_R = TIMER_ICR_TATOCINT;
	TIMER4_IMR_R = 0x00000001;
	NVIC_PRI17_R = (NVIC_PRI17_R&0xFF1FFFFF) | (pri<<21);
	NVIC_EN2_R |= 1<<(70-64);
	TIMER4_TAILR_R = period;
	TIMER4_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void TIMER0A_Stop(void){
	if(Clock0){
		TIMER0_CTL_R = 0;
		Clock0 = false;
	}
}

void TIMER1A_Stop(void){
	if(Clock1){
		TIMER1_CTL_R = 0;
		Clock1 = false;
	}
}

void TIMER2A_Stop(void){
	if(Clock2){
		TIMER2_CTL_R = 0;
		Clock2 = false;
	}
}

void TIMER3A_Stop(void){
	if(Clock3){
		TIMER3_CTL_R = 0;
		Clock3 = false;
	}
}

void TIMER4A_Stop(void){
	if(Clock4){
		TIMER4_CTL_R = 0;
		Clock4 = false;
	}
}

void TIMER0A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	Task0();
}

void TIMER1A_Handler(void){
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	Task1();
}

void TIMER2A_Handler(void){
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;
	Task2();
}

void TIMER3A_Handler(void){
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;
	Task3();
}

void TIMER4A_Handler(void){
	TIMER4_ICR_R = TIMER_ICR_TATOCINT;
	Task4();
}
