

#ifndef ANIMATION_H
#define ANIMATION_H
#include <stdint.h>
#include "Images.h"
class Animation {
  private:
    uint8_t numFrames;
    uint8_t frame;
    const unsigned short* const * animation;
  public:
  Animation(const unsigned short* const* anim,uint8_t nf);
  const unsigned short* getFrame(void);
  void reset(void);
  void nextframe(void);
};


#endif 
