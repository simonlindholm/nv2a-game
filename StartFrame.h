#pragma once
#include "Frame.h"

class StartFrame : public Frame {
	public:
		StartFrame();
		Frame* frame(SDL_Surface* screen, unsigned int delay);
};
