#pragma once
#include <vector>
#include "Player.h"
#include "shared_ptr.h"
#include "util.h"

// Implements a simple, stupid, opponent player
// (this file is to be removed later)
class StupidAIPlayer : public Player {
	private:
		std::vector<Coord> checkpoints;
		size_t moveInd;
		int targetInd;
		int shootDelay;
	public:
		StupidAIPlayer(const std::vector<Coord>& checkpoints);
		virtual Action move(const GameState& game, unsigned int delay);
};