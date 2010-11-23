#pragma once
#include <vector>
#include <list>
#include "Hitbox.h"
#include "Timer.h"
#include "Level.h"
#include "fwd.h"
#include "shared_ptr.h"

// The state of a game, such as player and item locations, the map used, etc.
struct GameState {
	Level level;

	std::vector<shared_ptr<Player> > players;
	std::vector<PlayerInfo> playerInfo;
	std::list<shared_ptr<Bullet> > bullets;

	std::list<shared_ptr<Item> > items;
	Timer itemTimer;
	int itemsLeft;
};
