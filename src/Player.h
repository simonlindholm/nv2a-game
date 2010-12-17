#pragma once
#include <list>
#include "fwd.h"
#include "util.h"
#include "Hitbox.h"
#include "Timer.h"
#include "Buff.h"
#include "shared_ptr.h"

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
		Timer shotTimer;
		std::list<shared_ptr<Buff> > buffs;

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

		// Restart the shot timer
		void restartShotTimer();

		// Check if the shot timer is done
		bool canShoot();

		// Add a buff to the list of currently active buffs
		void addBuff(shared_ptr<Buff> buff);

		// Resets all stats, then reactivates all buffs
		void recalculateBuffs();

		// Performs relevant player-related logic during a frame, 
		// such as regenerating HP and decreasing buff timers.
		void step(unsigned int delay);
};

// An abstract class describing the logic of a player (instantiated by classes
// like StupidAIPlayer, HumanPlayer, etc.)
class PlayerLogic {
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

		virtual ~PlayerLogic() = 0;

		// Returns what actions the player performs this frame, given the state
		// of the game and the delay since last frame
		virtual Action move(const GameState& game, unsigned int delay) = 0;

		// Signal to the player that they have respawned. (Note: The
		// signaling system should probably be changed to use events when
		// it grows big enough.)
		virtual void signalSpawn();

		// Initialize the external state of a player. Must be called before
		// any other function is used, and may not be called twice.
		void initInfo(const PlayerInfo* info);
};

// A class describing a player, holds the info and the logic objects
class Player {
	public:
		PlayerInfo info;
		shared_ptr<PlayerLogic> logic;
		Player(shared_ptr<PlayerLogic> plogic);
	private:
		// Copy constructor and assignment operator are left undefined since
		// Player has no copy semantics
		Player(const Player&); 
		Player& operator=(const Player&);
};
