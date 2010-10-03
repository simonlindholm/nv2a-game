#pragma once
#include "GameState.h"
#include "util.h"

// An abstract class describing a player in the game
class Player {
	private:
		// The position of the center of the player
		Coord pos;

		// The angle of sight
		double angle;

		// XXX: Some sort of hitbox needed here

	protected:
		// Set the angle of sight (from the derived class)
		void setAngle(double angle);

	public:
		// A struct describing the actions a player performs at a certain frame,
		// such as moving, shooting, etc.
		struct Action {
			double mx, my;
			bool shooting;
			double shootingAngle;
		};

		Player();

		// (Every abstract class needs a virtual destructor; I can explain this in person later)
		virtual ~Player() = 0;

		// Returns what actions the player performs this frame, given the state of
		// the game and the delay since last frame
		virtual Action move(GameState* game, unsigned int delay) = 0;

		// Get the player's position
		Coord getPosition() const;

		// Move a player by (x, y). This should only be called from some managing
		// class, such as GameState.
		void moveBy(double x, double y);

		// Get the angle of movement
		double getAngle() const;
};
