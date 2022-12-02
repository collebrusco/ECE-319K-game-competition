// SlidePot.cpp
// Runs on TM4C123
// SlidePot object definitions
// Designed for franks ECE319K PCB 
// Created: 11/15/22
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/15/22

#include <stdint.h>
#include "SlidePot.h"
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: sac register value, hardware will avg 2^sac samples
// Output: none
// measures from PD2, analog channel 5
static void ADC0_Init(uint32_t sac){ 
	SYSCTL_RCGCGPIO_R |= 0x08;
	SYSCTL_RCGCADC_R |= 0x01;
	while (!(SYSCTL_PRADC_R & 0x01)){}
	volatile uint32_t delay = 666; 
	GPIO_PORTD_DEN_R &= ~(0x00000004);
	GPIO_PORTD_AFSEL_R |= 0x00000004;
	GPIO_PORTD_AMSEL_R |= 0x00000004;
	GPIO_PORTD_DIR_R &= ~(0x00000004);
	
	ADC0_PC_R = ADC_PC_SR_500K; // ~2us
	ADC0_SSPRI_R = 0x1203; //set seq priorities, Slide < Joy
	ADC0_ACTSS_R &= ~(ADC_ACTSS_ASEN3);//turn of seq 3 for init
	ADC0_EMUX_R &= ~(ADC_EMUX_EM3_M);//SW start
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R & ~(0x0F)) | 0x05; // channel 5, PD2
	ADC0_SSCTL3_R = 0x06; // 0110 => not temp, set flag, stop, not differential
	ADC0_IM_R &= ~(ADC_IM_MASK3); //dont interrupt
	if ((sac < 7) && (sac > 0)){
		ADC0_SAC_R = sac;
	}else{
		ADC0_SAC_R = 0;
	}
	ADC0_ACTSS_R |= ADC_ACTSS_ASEN3; // turn seq 3 back on
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
static uint32_t ADC0_In(void){  
	uint32_t data;
	ADC0_PSSI_R = 0x08;
	while (!(ADC0_RIS_R & 0x08)){
	}
	data = ADC0_SSFIFO3_R;
	ADC0_ISC_R = 8;
  return data;
}

SlidePot::SlidePot(uint8_t sac){
	ADC0_Init(sac);
}

SlidePot::~SlidePot(){
	SYSCTL_RCGCGPIO_R &= ~(0x08);
	ADC0_ACTSS_R &= ~(ADC_ACTSS_ASEN3);
}

void SlidePot::disableADC0(void){
	SYSCTL_RCGCADC_R &= ~(0x01);
}

void SlidePot::sample(void){
	data = ADC0_In();
	flag = 1;
}

uint32_t SlidePot::getData(void){
	if (data < 20){
		return 4096;
	}
  return 4095-data;
}

int32_t SlidePot::getSignedData(void){
  return ((int32_t)this->getData())-2048;
}

int32_t SlidePot::scaleByData(int32_t n){
	return ((this->getSignedData()) * n)>>11;
}

int32_t SlidePot::uScaleByData(int32_t n){
	return ((this->getData()) * n)>>12;
}

