#pragma once
#include <SDL/SDL.h>
#include <string>
#include "util.h"

SDL_Surface* loadSurface(const char* filename);
void drawCenteredSurface(const Coord& position, SDL_Surface* image, SDL_Surface* screen);

struct RotatableObject {
	SDL_Surface* images[360]; //Holds one image for every angle
};
