#pragma once
#include "fwd.h"
#include "util.h"
#include "Hitbox.h"

// An abstract class describing a player in the game
class Player {
	private:
		Coord pos;
		double angle;
		double speed;
		Hitbox hitbox;

	protected:
		// Set the angle of sight (from the derived class)
		void setAngle(double angle);

	public:
		// A struct describing the actions a player performs at a certain frame,
		// such as moving, shooting, etc.
		struct Action {
			double mx, my;
			bool shooting;
		};

		Player();

		virtual ~Player() = 0;

		// Returns what actions the player performs this frame, given the state of
		// the game and the delay since last frame
		virtual Action move(const GameState& game, unsigned int delay) = 0;

		// Get the player's position
		Coord getPosition() const;

		// Move the player to a position. This should only be called from some
		// managing class, such as GameState.
		void moveTo(const Coord& to);

		// Get the angle of movement
		double getAngle() const;

		// Get the speed of the player, in pixels per millisecond
		double getSpeed() const;

		// Get the (external) hitbox for the player
		Hitbox getHitbox() const;
};
