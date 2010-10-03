#include "SDL_helpers.h"
#include "exceptions.h"

void SDL_SimpleLock(SDL_Surface* sur) {
	if (SDL_MUSTLOCK(sur)) {
		if (SDL_LockSurface(sur)) {
			throw SDL_Exception();
		}
	}
}

void SDL_SimpleUnlock(SDL_Surface* sur) {
	if (SDL_MUSTLOCK(sur)) {
		SDL_UnlockSurface(sur);
	}
}

int SDL_FillRectLocked(SDL_Surface* dst, Uint8 r, Uint8 g, Uint8 b) {
	SDL_SimpleLock(dst);
	Uint32 color = SDL_MapRGB(dst->format, r, g, b);
	int ret = SDL_FillRect(dst, 0, color);
	SDL_SimpleUnlock(dst);
	return ret;
}

SDL_Lock::SDL_Lock(SDL_Surface* s) : sur(s) {
	SDL_SimpleLock(sur);
}

SDL_Lock::~SDL_Lock() {
	SDL_SimpleUnlock(sur);
}
