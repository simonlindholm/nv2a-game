#pragma once
#include "Frame.h"
#include "Player.h"
#include "shared_ptr.h"

class GameFrame : public Frame {
	public:
		GameFrame(shared_ptr<Player> players);
		Frame* frame(SDL_Surface* screen, unsigned int delay);
};
