// fmath.h
// generic math library stuff
// github.com/collebrusco
#ifndef FMATH_H
#define FMATH_H
#include <stdint.h>
// 256 samples from 0 to 25.5

// absolute value
int32_t abs(int32_t);

// maximum of 2
int32_t max(int32_t, int32_t);

// minimum of 2
int32_t min(int32_t, int32_t);

int32_t amax(int32_t, int32_t);

int32_t amin(int32_t, int32_t);

int32_t limitMagnatude(int32_t, uint32_t);

#endif
