#pragma once
#include <vector>
#include "fwd.h"
#include <list>
#include "shared_ptr.h"

// The state of a game, such as player and item locations, the map used, etc.
struct GameState {
	std::vector<shared_ptr<Player> > players;
	std::list<shared_ptr<Bullet> > bullets;
	std::list<shared_ptr<Item> > items;
	Hitbox wall;
};
