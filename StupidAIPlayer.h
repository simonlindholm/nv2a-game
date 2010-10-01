#pragma once
#include "Player.h"

// Implements a simple, stupid, opponent player
// (this file is to be removed later)
class StupidAIPlayer : public Player {
	public:
		Action move(GameState* game, unsigned int delay);
};
