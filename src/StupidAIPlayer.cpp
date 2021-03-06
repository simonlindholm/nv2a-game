#include <vector>
#include <cmath>
#include <cfloat>
#include <cstdlib>
#include "StupidAIPlayer.h"
#include "GameState.h"
#include "util.h"
#include "mathutil.h"

StupidAIPlayer::StupidAIPlayer(const std::vector<Coord>& checkpoints)
	: checkpoints(checkpoints), moveInd(0)
{
	// shootDelay and disDelay will get initialized on spawn, and targetInd
	// will be set when the game begins, so we don't need to initialize those.
}

void StupidAIPlayer::startGame(const GameState& game) {
	for (size_t i = 0; i < game.players.size(); ++i) {
		if (game.players[i]->logic.get() != this) {
			targetInd = i;
			break;
		}
	}
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
	Coord pos = info->getPosition();
	Action ret;

	if (disDelay >= 800) {
		// Player is sufficiently disoriented, don't move at all
		ret.mx = ret.my = 0;
	}
	else {
		// Move normally
		double mov = info->getSpeed() * delay, dist, relX, relY;
		Coord towards, fromp = pos;
		ret.mx = ret.my = 0;

		// Walk the checkpoint cycle until we exhaust the distance we can move.
		for (;;) {
			towards = checkpoints[moveInd];

			relX = towards.x - fromp.x;
			relY = towards.y - fromp.y;
			dist = pyth(relX, relY);

			// If the target cannot be reached, break out of the loop
			if (dist > mov) break;

			// Reduce the distance we can move by walking to the the next
			// checkpoint, and set the target to the next one.
			mov -= dist;
			fromp = towards;
			ret.mx += relX;
			ret.my += relY;
			if (++moveInd == checkpoints.size()) {
				moveInd = 0;
			}
		}

		double rat = mov / dist;
		ret.mx += relX * rat;
		ret.my += relY * rat;
	}

	// Calculate the angle of sight
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
