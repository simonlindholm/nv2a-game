#include "SDL_helpers.h"
#include "exceptions.h"

int SDL_FillRectLocked(SDL_Surface* dst, Uint8 r, Uint8 g, Uint8 b) {
	if (SDL_MUSTLOCK(dst)) {
		if (SDL_LockSurface(dst)) {
			throw SDL_Exception();
		}
	}
	Uint32 color = SDL_MapRGB(dst->format, r, g, b);
	int ret = SDL_FillRect(dst, 0, color);
	if (SDL_MUSTLOCK(dst)) {
		SDL_UnlockSurface(dst);
	}
	return ret;
}
