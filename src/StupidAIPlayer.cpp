#include <vector>
#include <cmath>
#include <cstdlib>
#include "StupidAIPlayer.h"
#include "GameState.h"
#include "util.h"
#include "mathutil.h"

StupidAIPlayer::StupidAIPlayer(const std::vector<Coord>& checkpoints)
	: checkpoints(checkpoints), moveInd(0), targetInd(-1)
{
	this->shootDelay = randRange(1500, 2500);
}

// Move the player in some (rather stupid) way
PlayerLogic::Action StupidAIPlayer::move(const GameState& game, unsigned int delay) {
	Action ret;

	Coord towards = this->checkpoints[this->moveInd];
	Coord pos = info->getPosition();

	double mov = info->getSpeed() * delay;
	double relX = towards.x - pos.x;
	double relY = towards.y - pos.y;
	double dist = pyth(relX, relY);

	if (dist <= mov) {
		// Player can reach target in this step, teleport there
		ret.mx = relX;
		ret.my = relY;
		if (++this->moveInd == this->checkpoints.size()) {
			this->moveInd = 0;
		}
	}
	else {
		// Move normally
		double rat = mov / dist;
		ret.mx = relX * rat;
		ret.my = relY * rat;
	}

	// Calculate the angle of sight
	if (targetInd == -1) {
		for (size_t i = 0; i < game.players.size(); ++i) {
			if (game.players[i]->logic.get() != this) {
				targetInd = static_cast<int>(i);
				break;
			}
		}
	}
	Coord targetPos = game.players[targetInd]->info.getPosition();
	double dx = targetPos.x - pos.x;
	double dy = targetPos.y - pos.y;
	ret.angle = std::atan2(-dy, dx);

	// Handle random shooting
	ret.shooting = false;
	this->shootDelay -= delay;
	if (this->shootDelay <= 0) {
		ret.shooting = true;
		this->shootDelay = randRange(200, 1500);
	}

	return ret;
}
