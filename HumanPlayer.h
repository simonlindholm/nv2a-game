#pragma once
#include "Player.h"

class HumanPlayer : public Player {
	public:
		Action move(GameState* game, unsigned int delay);
};
