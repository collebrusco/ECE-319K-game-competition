
#include <stdint.h>

#include "Animation.h"

Animation::Animation(const unsigned short* const* anim,uint8_t nf){
	animation = anim;
	numFrames = nf;
	frame = 0;
}
const unsigned short* Animation::getFrame(void){
	return animation[frame];
}
void Animation::nextframe(void){
	frame = (frame + 1) % numFrames;
}

void Animation::reset(void){
	frame = 0;
}
