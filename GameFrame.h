#pragma once
#include "Frame.h"
#include "Player.h"
#include "shared_ptr.h"

// This is the 'game' state, where most of the logic should be located
class GameFrame : public Frame {
	public:
		GameFrame(shared_ptr<Player> enemy);
		Frame* frame(SDL_Surface* screen, unsigned int delay);
};
