// trig.h
// necessary trig tables for franks ece319k PCB
// created using trig sampler c++ programs
// find those on my github
// github.com/collebrusco
#ifndef TRIG_H
#define TRIG_H
#include <stdint.h>
// 256 samples from 0 to 25.5

//========ATAN========
//Input: y/x , fixed point at 0.1 resolution
//Output: result, -pi/2 <= result <= pi/2
//output in 2^-10 fixed point
int32_t atan(int32_t in);


#endif
