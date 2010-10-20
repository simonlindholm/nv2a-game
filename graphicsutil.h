#pragma once
#include <SDL/SDL.h>
#include "string"
#include "util.h"
#include <vector>

using namespace std;

SDL_Surface* load_surface(string filename);
void draw_surface(Coord position, SDL_Surface* image, SDL_Surface* screen);

struct rotateableObject
{
    SDL_Surface* images[360]; //Holds one image for every angle
};
