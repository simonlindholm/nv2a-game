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

		// Resets the player's stats to their default values
		void resetStats();

		// Get the angle of movement
		double getAngle() const;

		// Set the player's speed. Called when buffing the player.
		void setSpeed(double spe);

		// Get the speed of the player, in pixels per millisecond
		double getSpeed() const;

		// Get the player's position
		Coord getPosition() const;

		// Set the player's position
		void moveTo(const Coord& to);

		// Set HP based on bullet collision, HP regeneration or HP restoration
		void setHP(int newHP);

		// Get the current HP of the player
		int getHP() const;

		// Set the player's defense. Called when buffing the player.
		void setDef(int newDef);

		// Get the player's current defense stat
		int getDef() const;

		// Get the (external) hitbox for the player
		const Hitbox& getHitbox() const;

		// Set the angle of sight
		void setAngle(double angle);

		// Resets HP regeneration timers
		void resetRegenTimer();

		// Regenerate 15 HP per second after 6 seconds of not being hit.
		// The delay is used to step the appropriate regeneration timer.
		void regenTimerLogic(unsigned int delay);

		// Set the buff timer
		void setBuffTimer(unsigned int time);

		// When the buff timer is done, stop the timer and call resetStats()
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
