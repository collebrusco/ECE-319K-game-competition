// JoyStick.h
// Runs on TM4C123
// JoyStick object 
// Designed for franks ECE319K PCB 
// Created: 11/15/22
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/15/22



// Call sample() to update data
// Call x() or y() to get data from -1 to 1 in 2^-11 fixed point
// Call scaleBy(n) to scale n by -1 to 1 depending on data
// Call sw() to get state of switch

#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <stdint.h>
#include "../flib/trig.h"
class JoyStick{ 
private:
	uint32_t deadzone;
  	uint32_t xData;     // raw ADC value, X
	uint32_t yData;			// raw ADC value, Y
	bool button;				// joystick clicked in
	bool buttonPrev;		// joystick was clicked on prev sample
  	int32_t flag;       // 0 if data is not valid, 1 if valid
public:
  JoyStick(uint32_t dz); 			// initialize ADC & slide pot, input is deadzone
	~JoyStick(void);			// disable GPIO clock & sequencer
	void hardwareAVG(uint32_t sac); // add adc averaging, slow
	void disableADC0(void); //disable ADC0 (WARNING -- Slidepot uses it too);
  void sample(void);    // sample ADC
  int32_t x(void);     	// return horizontal ADC sample value (-2048 to 2047)
	int32_t y(void);			// return vertical ADC sample value (-2048 to 2047)
	bool sw(void);				// return whether switch is pressed (true or false)
	bool sw_ris(void);		// detect rising edge of joy switch
	int32_t scaleByX(int32_t n);	// return n * xdata, -1 <= xdata <= 1
	int32_t scaleByY(int32_t n);	// return n * ydata, -1 <= ydata <= 1
	uint32_t mag(void);	// return length of (x,y) vector @ 2^11 res
//	int32_t dir(void);	// return angle joy is pointing in 0 to 360 deg
};										// where 0 is 3 o'clock and increasing angles move counter-cw
											// behavior is undefined when joy is in center, check mag

// ADC initialization function, channel 5, PD2
// Input: none
// Output: none
//void ADC_Init(void);

//------------ADC_In------------
// Busy-wait Analog to digital conversion, channel 5, PD2
// Input: none
// Output: 12-bit result of ADC conversion
//uint32_t ADC_In(void);

#endif
