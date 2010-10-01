#include "StupidAIPlayer.h"

// Move the player in some (rather stupid) way
Player::Action StupidAIPlayer::move(GameState* game, unsigned int delay) {
	// Currently, the AI just moves right as far as possible.
	// XXX: Change this to something slightly less stupid, such as
	// alternating between two (or more) places and shooting randomly
	// in the direction of the player.
	Action ret;
	ret.mx = 1;
	ret.my = 0;
	ret.shooting = false;
	return ret;
}
