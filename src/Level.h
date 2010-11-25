#pragma once
#include <SDL/SDL.h>
#include <vector>
#include "util.h"
#include "Hitbox.h"

struct Level {
	SDL_Surface* bg;
	Hitbox wall;
	std::vector<Coord> startPoints;
	std::vector<Coord> spawnPoints;
};
