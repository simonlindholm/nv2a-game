#include <vector>
#include <cmath>
#include "StupidAIPlayer.h"
#include "util.h"

StupidAIPlayer::StupidAIPlayer(const std::vector<Coord>& checkpoints) 
	: checkpoints(checkpoints), moveInd(0)
{
}

// Move the player in some (rather stupid) way
Player::Action StupidAIPlayer::move(const GameState& game, unsigned int delay) {
	Action ret;

	Coord towards = this->checkpoints[this->moveInd];
	Coord pos = this->getPosition();

	double mov = this->getSpeed() * delay;
	double relX = towards.x - pos.x;
	double relY = towards.y - pos.y;
	double dist = std::sqrt(relX*relX + relY*relY);

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

	if (!(fpEqual(ret.mx, 0, 1) && fpEqual(ret.my, 0, 1))) {
		double angle = atan2(-ret.my, ret.mx);
		if (angle < 0) angle += 2*m_pi;
		this->setAngle(angle);
	}

	ret.shooting = false;
	ret.shootingAngle = 0;
	return ret;
}
