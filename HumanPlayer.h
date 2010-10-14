#pragma once
#include <SDL/SDL.h>
#include "Player.h"
#include "util.h"

// Implements a player controlled by the user
class HumanPlayer : public Player {
	private:
		int vx, vy;
		Coord mouse;
		bool shooting;
	public:
		HumanPlayer();
		void startFrame(Uint8* keyState, Uint8 mouseState, int mouseX, int mouseY);
		void handleEvent(const SDL_Event& event);
		Action move(const GameState& game, unsigned int delay);
		void paint(const GameState& game, SDL_Surface* screen);
};
