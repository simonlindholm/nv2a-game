#pragma once
#include <SDL/SDL.h>

// Fill a surface with a color (r, g, b) with lock if needed
int SDL_FillRectLocked(SDL_Surface* dst, Uint8 r, Uint8 g, Uint8 b);

// Lock a surface if needed, and throw a SDL_Exception in case of error
void SDL_SimpleLock(SDL_Surface* sur);

// Unlock a surface if needed
void SDL_SimpleUnlock(SDL_Surface* sur);

class SDL_Lock {
	private:
		SDL_Surface* sur;

		// Declare default constructors and assignment operator as private
		// (without a definition) to avoid invalid locks/duplicates.
		SDL_Lock();
		SDL_Lock(const SDL_Lock&);
		SDL_Lock& operator=(const SDL_Lock&);
	public:
		SDL_Lock(SDL_Surface* s);
		~SDL_Lock();
};
