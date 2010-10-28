#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include "graphicsutil.h"
#include "exceptions.h"
using namespace std;

SDL_Surface* loadSurface(const std::string& filename) {
	SDL_Surface* loadedImage = IMG_Load(filename.c_str());
	if (!loadedImage) throw SDL_Exception();

	//Optimize format
	SDL_Surface* ret = SDL_DisplayFormatAlpha(loadedImage);
	SDL_FreeSurface(loadedImage);
	if (!ret) throw SDL_Exception();

	return ret;
}

void drawCenteredSurface(const Coord& position, SDL_Surface* image, SDL_Surface* screen) {
	//Rectangle to hold the position as SDL_BlitSurface only takes SDL_Rect
	SDL_Rect pos;
	pos.x = static_cast<Sint16>(position.x - image->w/2.0);
	pos.y = static_cast<Sint16>(position.y - image->h/2.0);

	//Draw the surface
	SDL_BlitSurface(image, 0, screen, &pos);
}
