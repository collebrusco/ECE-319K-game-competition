// flib/Timers.cpp
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
long StartCritical(void);
void EndCritical(long sr);

void Timer0A_Init(uint32_t period){
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x01; //activate T0
	{volatile int i = 0;}
	TIMER0_CTL_R = 0;
	TIMER0_CFG_R = 0;
	TIMER0_TAMR_R = 0x00000002;
	TIMER0_TAPR_R = 0;
	TIMER0_TAILR_R = period;
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	TIMER0_IMR_R = 0x00000001;
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF);
	NVIC_EN0_R |= (1<<19);
	TIMER0_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void Timer1A_Init(uint32_t period){
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x02; //activate T1
	{volatile int i = 0;}
	TIMER1_CTL_R = 0;
	TIMER1_CFG_R = 0;
	TIMER1_TAMR_R = 0x00000002;
	TIMER1_TAPR_R = 0;
	TIMER1_TAILR_R = period;
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	TIMER1_IMR_R = 0x00000001;
	NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF);
	NVIC_EN0_R |= (1<<21);
	TIMER1_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void Timer2A_Init(uint32_t period){
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x04; //activate T2
	{volatile int i = 0;}
	TIMER2_CTL_R = 0;
	TIMER2_CFG_R = 0;
	TIMER2_TAMR_R = 0x00000002;
	TIMER2_TAPR_R = 0;
	TIMER2_TAILR_R = period;
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;
	TIMER2_IMR_R = 0x00000001;
	NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF);
	NVIC_EN0_R |= (1<<23);
	TIMER2_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void Timer3A_Init(uint32_t period){
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x08; //activate T3
	{volatile int i = 0;}
	TIMER3_CTL_R = 0;
	TIMER3_CFG_R = 0;
	TIMER3_TAMR_R = 0x00000002;
	TIMER3_TAPR_R = 0;
	TIMER3_TAILR_R = period;
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;
	TIMER2_IMR_R = 0x00000001;
	NVIC_PRI5_R = (NVIC_PRI8_R&0x00FFFFFF);
	NVIC_EN1_R |= (1<<(35-32));
	TIMER3_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void Timer4A_Init(uint32_t period){
	long sr = StartCritical();
	SYSCTL_RCGCTIMER_R |= 0x10; //activate T4
	{volatile int i = 0;}
	TIMER4_CTL_R = 0;
	TIMER4_CFG_R = 0;
	TIMER4_TAMR_R = 0x00000002;
	TIMER4_TAPR_R = 0;
	TIMER4_TAILR_R = period;
	TIMER4_ICR_R = TIMER_ICR_TATOCINT;
	TIMER4_IMR_R = 0x00000001;
	NVIC_PRI17_R = (NVIC_PRI17_R&0xFF1FFFFF);
	NVIC_EN2_R |= 1<<(70-64);
	TIMER4_TAILR_R = period;
	TIMER4_CTL_R |= 0x00000001;
	EndCritical(sr);
}

void TIMER0A_Stop(void){
	TIMER0_CTL_R = 0;
}

void TIMER1A_Stop(void){
	TIMER1_CTL_R = 0;
}

void TIMER2A_Stop(void){
	TIMER2_CTL_R = 0;
}

void TIMER3A_Stop(void){
	TIMER3_CTL_R = 0;
}

void TIMER4A_Stop(void){
	TIMER4_CTL_R = 0;
}
