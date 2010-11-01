#pragma once
#include "fwd.h"
#include "util.h"
#include "Hitbox.h"
#include "Timer.h"

// A class describing the external state of the player
class PlayerInfo {
	private:
		double speed;
		int hp;
		Hitbox hitbox, rhitbox;
		mutable Hitbox phitbox;
		Coord pos;
		double angle;
		Timer regenTimer;

	public:
		PlayerInfo();

		// Get the angle of movement
		double getAngle() const;

		// Get the speed of the player, in pixels per millisecond
		double getSpeed() const;

		// Get the player's position
		Coord getPosition() const;

		// Move the player to a position. This should only be called from some
		// managing class, such as GameState.
		void moveTo(const Coord& to);

		// Set HP based on bullet collision or HP regeneration
		void setHP(int newHP);

		// Get the current HP of the player
		int getHP() const;

		// Get the (external) hitbox for the player
		const Hitbox& getHitbox() const;

		// Set the angle of sight (from the derived class)
		void setAngle(double angle);

		// Step regenTimer by delay ms, then regenerate HP if timer is done
		void regenTimerLogic(unsigned int delay);
};

// An abstract class describing a player in the game
class Player {
	protected:
		// The external state of the player
		const PlayerInfo* info;

	public:
		// A struct describing the actions a player performs at a certain frame,
		// such as moving, shooting, etc.
		struct Action {
			double mx, my;
			bool shooting;
			double angle;
		};

		virtual ~Player() = 0;

		// Returns what actions the player performs this frame, given the state
		// of the game and the delay since last frame
		virtual Action move(const GameState& game, unsigned int delay) = 0;

		// Initialize the external state of a player. Must be called before
		// any other function is used.
		void setInfo(const PlayerInfo* info);

};
