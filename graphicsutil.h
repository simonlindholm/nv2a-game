#pragma once
#include <SDL/SDL.h>
#include "string"
#include "util.h"

SDL_Surface* loadSurface(const std::string& filename);
void drawCenteredSurface(const Coord& position, SDL_Surface* image, SDL_Surface* screen);

struct RotateableObject
{
    SDL_Surface* images[360]; //Holds one image for every angle
};
