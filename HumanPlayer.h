#pragma once
#include "Player.h"

// Implements a player controlled by the user
class HumanPlayer : public Player {
	public:
		//TODO: Add functions for receiving input from GameFrame
		Action move(GameState* game, unsigned int delay);
};
