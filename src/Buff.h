#pragma once
#include <SDL/SDL.h>
#include "Timer.h"
#include "Resources.h"
#include "fwd.h"

// Abstract class symbolizing a buff
class Buff {
	private:
		Timer buffTimer;
	public:
		Buff(unsigned int time);

		virtual ~Buff();

		// Step the buff timer by delay ms
		void step(unsigned int delay);

		// Check if a buff has ended
		bool hasEnded() const;

		// Activate a buff
		virtual void activate(PlayerInfo& pl) = 0;

		// Get the buff icon
		virtual StaticImages::Id getIcon() const = 0;

		// Get the indicator for the buff
		virtual SDL_Surface* getPlayerAura() const = 0;
};

class ShieldBuff : public Buff {
	public:
		ShieldBuff(PlayerInfo& pl);
		virtual void activate(PlayerInfo& pl);
		virtual StaticImages::Id getIcon() const;
		virtual SDL_Surface* getPlayerAura() const;
};

class SpeedBuff : public Buff {
	public:
		SpeedBuff(PlayerInfo& pl);
		virtual void activate(PlayerInfo& pl);
		virtual StaticImages::Id getIcon() const;
		virtual SDL_Surface* getPlayerAura() const;
};
