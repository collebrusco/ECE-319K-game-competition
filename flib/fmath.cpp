#include "fmath.h"
// fmath.cpp
// github.com/collebrusco

// 256 samples from 0 to 25.5

int32_t abs(int32_t x){
  if (x>=0){
    return x;
  } else {
    return -x;
  }
}

int32_t max(int32_t a, int32_t b){
    if (a > b){
        return a;
    }
    return b;
}

int32_t min(int32_t a, int32_t b){
    if (a < b){
        return a;
    }
    return b;
}

int32_t amax(int32_t a, int32_t b){
  bool aneg = (a < 0);
  bool bneg = (b < 0);
  a = abs(a);
  b = abs(b);
  if (a > b){
    return aneg ? -a : a;
  } else {
    return bneg ? -b : b;
  }
}

int32_t amin(int32_t a, int32_t b){
  bool aneg = (a < 0);
  bool bneg = (b < 0);
  a = abs(a);
  b = abs(b);
  if (a < b){
    return aneg ? -a : a;
  } else {
    return bneg ? -b : b;
  }
}

int32_t limitMagnatude(int32_t a, uint32_t max){
  if (abs(a) <= max){
    return;
  } else {
    if (a < 0){
      return -max;
    }
    return max;
  }
}
