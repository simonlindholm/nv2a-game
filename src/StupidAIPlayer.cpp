#include <vector>
#include <cmath>
#include <cfloat>
#include <cstdlib>
#include "StupidAIPlayer.h"
#include "GameState.h"
#include "util.h"
#include "mathutil.h"

StupidAIPlayer::StupidAIPlayer(const std::vector<Coord>& checkpoints)
	: checkpoints(checkpoints), moveInd(0), targetInd(-1)
{
	// shootDelay and disDelay will get initialized on spawn, so do nothing.
}

void StupidAIPlayer::signalSpawn() {
	// Set the AI to move to the nearest checkpoint, this makes a good
	// heuristic for not getting stuck in things.
	double nDist = DBL_MAX;
	Coord pos = info->getPosition();
	for (size_t i = 0; i < checkpoints.size(); ++i) {
		Coord p = checkpoints[i];
		double dsq = distsq(pos, p);
		if (dsq < nDist) {
			nDist = dsq;
			this->moveInd = (int)i;
		}
	}

	// Let the AI become disDelay by spawning, and not shoot immediately
	shootDelay = 700;
	disDelay = 1000;
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

	if (disDelay >= 800) {
		// Player is sufficiently disoriented, don't move at all
		ret.mx = ret.my = 0;
	}
	else if (dist <= mov) {
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

	if (disDelay > 0) {
		// Introduce some errors in the aim when disoriented
		ret.angle += disDelay * std::sin(disDelay / 260.0) / 1600.0;

		// Step the disorientation timer
		disDelay -= delay;
	}

	// Handle random shooting
	ret.shooting = false;
	this->shootDelay -= delay;
	if (this->shootDelay <= 0) {
		ret.shooting = true;
		this->shootDelay += randRange(200, 500);
	}

	return ret;
}
