#pragma once
#include "Frame.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "GameState.h"
#include "shared_ptr.h"

// This is the 'game' state, where most of the logic should be located
class GameFrame : public Frame {
	private:
		GameState gameState;
		shared_ptr<HumanPlayer> player;
		// XXX: Merge into one array containing all players
		shared_ptr<Player> enemy;
	public:
		GameFrame(shared_ptr<Player> enemy);
		Frame* frame(SDL_Surface* screen, unsigned int delay);
};
