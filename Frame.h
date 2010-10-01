#pragma once
#include <SDL/SDL.h>

// Frame is an abstract class symbolising a game state, such as 'menu',
// 'highscore' or 'game'.
class Frame {
	public:
		virtual ~Frame() = 0; // (explained elsewhere)

		// frame() should calculate the new internal state of the Frame after a delay of
		// 'delay' milliseconds, painting the result to 'screen'. It is also responsible
		// for handling all events that happen while the particular Frame is active.
		// A non-null return value means to switch active state to the returned Frame (and
		// deleting the previous), and a return value of 0 means to keep the current Frame.
		virtual Frame* frame(SDL_Surface* screen, unsigned int delay) = 0;
};
