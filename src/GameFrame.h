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
		void setItemTimer();
		shared_ptr<Item> getRandomItem() const;
	public:
		GameFrame(const Level& level, const std::vector<shared_ptr<PlayerLogic> >& enemies);
		virtual Frame* frame(SDL_Surface* screen, unsigned int delay);
};
