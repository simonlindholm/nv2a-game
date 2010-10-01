#pragma once
#include <SDL/SDL.h>

class Frame {
	public:
		virtual ~Frame() = 0;
		virtual Frame* frame(SDL_Surface* screen, unsigned int delay) = 0;
};
