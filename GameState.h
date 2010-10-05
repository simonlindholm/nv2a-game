#pragma once
#include <vector>
#include "fwd.h"
#include "shared_ptr.h"

// The state of a game, such as player and item locations, the map used, etc.
struct GameState {
	// TODO: Add stuff here
	std::vector<shared_ptr<Player> > players;
};
