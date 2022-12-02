// SlidePot.cpp
// Runs on TM4C123
// SlidePot object 
// Designed for franks ECE319K PCB 
// Created: 11/15/22
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/15/22

// {========SLIDEPOT========}
//left			 center			 right
// 0          2048			  4095    <- using getData()
//-2048        0          2047		<- using getSignedData()
// -n   -n/2   0    n/2     n     <- using scaleByData(n)

// call sample() to update data

#ifndef SLIDEPOT_H
#define SLIDEPOT_H
#include <stdint.h>

class SlidePot{ 
private:
  uint32_t data;     // raw ADC value
  int32_t flag;      // 0 if data is not valid, 1 if valid
public:
  SlidePot(uint8_t sac);// initializes ADC & GPIO. input is ADC SAC, ADC will average 2^sac measurements each sample
	~SlidePot(void);//disable GPIO clock and sequencer
	void disableADC0(void); //disable ADC0 (WARNING -- Joystick uses it too);
  void sample(void);        		// sample ADC
  uint32_t getData(void);     	// return ADC sample value (0 to 4095)
  int32_t getSignedData(void);  // return ADC sample centered at 0 (-2048 to 2047)
	int32_t scaleByData(int32_t);	// return ADC sample scaled (-1 to 1) 
	int32_t uScaleByData(int32_t);	// return ADC sample scaled (0 to 1) 
};

// ADC initialization function, channel 5, PD2
// Input: none
// Output: none
// private, used by class
//void ADC_Init(void);

//------------ADC_In------------
// Busy-wait Analog to digital conversion, channel 5, PD2
// Input: none
// Output: 12-bit result of ADC conversion
// private, used by class
//uint32_t ADC_In(void);

#endif
