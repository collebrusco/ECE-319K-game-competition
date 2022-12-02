#include "JoyStick.h"
#include "tm4c123gh6pm.h"
// JoyStick.cpp
// Runs on TM4C123
// Joystick Object Definitions
// Designed for franks ECE319K PCB
// Created: 11/15/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/15/22
/*
	Vertical axis on PE4, AIN8
	Horizontal ax on PE5, AIN9
*/
#include "../flib/trig.h"

extern "C" long StartCritical(void);
extern "C" void EndCritical(long);

// ADC init function
// Input: sac register value, hardware will average 2^n samples
// Output: none
// local and unique to joystick obj's needs
// ADC 0 on Sequencer 1 sampling channels 8 & 9
static void ADC0_Init(uint32_t sac){ 
	long sr = StartCritical();
	SYSCTL_RCGCGPIO_R |= 0x10;
	SYSCTL_RCGCADC_R |= 0x01;
	//while ((SYSCTL_PRADC_R & SYSCTL_PRADC_R0)!=SYSCTL_PRADC_R0){}
	for (int i = 100000; i; i--){}
	volatile uint32_t delay = 666; 
	GPIO_PORTE_DEN_R &= ~(0x00000030);
	GPIO_PORTE_AFSEL_R |= 0x00000030;
	GPIO_PORTE_AMSEL_R |= 0x00000030;
	GPIO_PORTE_DIR_R &= ~(0x00000030);
	
	ADC0_PC_R = ADC_PC_SR_500K; //125k/sec, ~8us
	ADC0_SSPRI_R = (ADC0_SSPRI_R & ~(0x3333)) | 0x1203; //set seq priorities, Joy > Slide
	ADC0_ACTSS_R &= ~(ADC_ACTSS_ASEN1);//turn of seq 1 for init
	ADC0_EMUX_R &= ~(ADC_EMUX_EM1_M);//SW start
	ADC0_SSMUX1_R = (ADC0_SSMUX1_R & ~(ADC_SSMUX1_MUX0_M))
									| (8<<ADC_SSMUX1_MUX0_S); // 1st sample channel 8, PE4
	ADC0_SSMUX1_R = (ADC0_SSMUX1_R & ~(ADC_SSMUX1_MUX1_M))
									| (9<<ADC_SSMUX1_MUX1_S); // 2nd sample channel 9, PE5
	ADC0_SSCTL1_R = ADC_SSCTL1_IE0 | ADC_SSCTL1_IE1 | ADC_SSCTL1_END1;
	ADC0_IM_R &= ~(ADC_IM_MASK1); //dont interrupt	
	if ((sac < 7) && (sac > 0)){
		ADC0_SAC_R = sac;
	}else{
		ADC0_SAC_R = 0;
	}
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN1; // turn seq 1 back on
	EndCritical(sr);
}
//	PF3 |= 0x08;
//	ADC0_In(yData, xData);
//	PF3 &= ~0x08;

#define PF3 (*((volatile uint32_t *)0x40025020))
#define PF2 (*((volatile uint32_t *)0x40025010))
#define PF1 (*((volatile uint32_t *)0x40025008))
static void ADC0_In(uint32_t &x, uint32_t &y){  
	ADC0_PSSI_R = ADC_PSSI_SS1;
	//PF3 |= 0x08;
	while (!(ADC0_RIS_R & ADC_RIS_INR1)){
	}
	//PF3 &= ~0x08;
	y = ADC0_SSFIFO1_R & 0xFFF;
	ADC0_ISC_R = ADC_ISC_IN1; //ex didnt have this clear in there...
	//PF2 |= 0x04;
//	while (!(ADC0_RIS_R & ADC_RIS_INR1)){
//	}
	//PF2 &= ~0x04;
	x = ADC0_SSFIFO1_R & 0xFFF;
	ADC0_ISC_R = ADC_ISC_IN1;
}


JoyStick::JoyStick(uint32_t dz){
	deadzone = dz;
	ADC0_Init(0);
	SYSCTL_RCGCGPIO_R |= 0x04;
	xData = 666;
	GPIO_PORTC_DEN_R |= 0x80;
	GPIO_PORTC_DIR_R &= ~(0x80);
}

void JoyStick::hardwareAVG(uint32_t sac){
	this->disableADC0();
	ADC0_Init(sac);
}

JoyStick::~JoyStick(void){
	ADC0_ACTSS_R &= ~(ADC_ACTSS_ASEN1);
}

void JoyStick::disableADC0(void){
	SYSCTL_RCGCADC_R &= ~(0x01);
}

void JoyStick::sample(void){
	ADC0_In(yData, xData);
	buttonPrev = button;
	long sr = StartCritical();
	button = (GPIO_PORTC_DATA_R & 0x80);
	EndCritical(sr);
}

int32_t JoyStick::x(void){
	int32_t res = -((int32_t)xData - 2048);
	if ((res < deadzone) || (-res < deadzone)){
		return 0;
	} else if((2048-res)<20){
		return 2048;
	} else {
		return res;
	}
}

int32_t JoyStick::y(void){
	int32_t res = ((int32_t)yData) - 2048;
	if ((res < deadzone) || (-res < deadzone)){
		return 0;
	} else if ((2048-res)<20){
		return 2048;
	} else {
		return res;
	}
}

bool JoyStick::sw(void){
	return button;
}

bool JoyStick::sw_ris(void){
	return button && !(buttonPrev);
}

int32_t JoyStick::scaleByX(int32_t n){
	return ((n * this->x()) >> 11);
}

int32_t JoyStick::scaleByY(int32_t n){
	return ((n * this->y()) >> 11);
}

uint32_t JoyStick::mag(void){
	int32_t mx = this->x();
	int32_t my = this->y();
	if (mx < 0){mx *= -1;}
	if (my < 0){my *= -1;}
	return (my > mx) ? my : mx;
}
/*
int32_t JoyStick::dir(void){
	int32_t d = atan((this->y()/this->x())*10);
	d += 1600;
	d *= 57;
	d >>= 10;
	
	return d;
}
*/





