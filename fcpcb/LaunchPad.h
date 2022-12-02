// LaunchPad.h
// Runs on TM4C123
// LaunchPad Object
// Designed for franks ECE319K PCB
// Created: 11/19/2022 
// Frank Collebrusco
// github.com/collebrusco
// LM: 11/20/22

#ifndef LAUNCHPAD_H
#define LAUNCHPAD_H
#include <stdint.h>

class LaunchPad {
private:
	bool lPrev;
	bool rPrev;
	bool L;
	bool R;
public:
    LaunchPad(void);
		void sample(void);
    //switches
    bool l(void); // left (top view, SW2 on board)
    bool r(void); // right(__________SW1_________)

    bool l_ris(void);
    bool r_ris(void);
    
};






#endif
