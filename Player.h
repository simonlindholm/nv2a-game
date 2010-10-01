#pragma once
#include "GameState.h"

class Player {
	public:
		// A struct describing the actions a player performs at a certain frame, such
		// as moving and shooting, etc.
		struct Action {
			double mx, my;
			bool shooting;
			double shootangle;
		};

		// (Every abstract class needs a virtual destructor; I can explain this in person later)
		virtual ~Player() = 0;

		// Returns what actions the player performs this frame, given the state of
		// the game and the delay since last frame
		virtual Action move(GameState* game, unsigned int delay) = 0;
};
