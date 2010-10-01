#include "StupidAIPlayer.h"

Player::Action StupidAIPlayer::move(GameState* game, unsigned int delay) {
	Action ret;
	ret.mx = 1;
	ret.my = 0;
	ret.shooting = false;
	return ret;
}
