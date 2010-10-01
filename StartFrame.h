#pragma once
#include "Frame.h"

// The state the program begins in, the only purpose of which is to immediately pass
// execution on to the real start frame (to decouple this choice from main().)
class StartFrame : public Frame {
	public:
		Frame* frame(SDL_Surface* screen, unsigned int delay);
};
