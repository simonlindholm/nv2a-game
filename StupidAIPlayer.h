#pragma once
#include "Player.h"

class StupidAIPlayer : public Player {
	public:
		Action move(GameState* game, unsigned int delay);
};
