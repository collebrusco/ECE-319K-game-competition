// DPad.h
// Runs on TM4C123
// DPad Object
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22

#ifndef DPAD_H
#define DPAD_H
#include <stdint.h>

class DPad {
private:
    bool aPrev;
    bool bPrev;
    bool xPrev;
    bool yPrev;
		bool A;
		bool B;
		bool X;
		bool Y;
public:
    DPad(void);
		void sample(void);
    bool a(void);       // return if sw currently pressed
    bool b(void);       // ____
    bool x(void);       // ____
    bool y(void);       // ____
    bool a_ris(void);   // detect rising edge of sw press
    bool b_ris(void);   // ____
    bool x_ris(void);   // ____
    bool y_ris(void);   // ____
};






#endif
