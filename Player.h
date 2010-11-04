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
		int defense;
		Hitbox hitbox, rhitbox;
		mutable Hitbox phitbox;
		Coord pos;
		double angle;
		Timer regenTimer, second;
		Timer buffTimer;

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

		// Set defense based on buffs from items
		void setDef(int newDef);

		// Get the player's current defense stat
		int getDef() const;

		// Get the (external) hitbox for the player
		const Hitbox& getHitbox() const;

		// Set the angle of sight (from the derived class)
		void setAngle(double angle);

		// Resets regenTimer to 5 seconds and second to 1 second
		void resetRegenTimer();

		// Step regenTimer (or second when regenTimer is done) by delay ms.
		// When both timers are done, player regains 15 hp and second resets.
		void regenTimerLogic(unsigned int delay);

		// Set the buff timer
		void setBuffTimer(unsigned int time);

		// Check if a buff is currently active or not
		bool buffIsActive() const;

		// Step buffTimer by delay ms. Stop timer when done.
		// If timer is done, revert any changes that have been done to Player's stats.
		void buffTimerLogic(unsigned int delay);
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
