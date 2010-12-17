#pragma once
#include "Timer.h"
#include "fwd.h"

// Abstract class symbolizing a buff
class Buff {
	private:
		Timer buffTimer;
	public:
		virtual ~Buff();

		// Set the buff timer
		void setBuffTimer(unsigned int time);

		// Step the buff timer by delay ms
		void step(unsigned int delay);

		// Check if a buff has ended
		bool hasEnded() const;

		// Activate a buff
		virtual void activate(PlayerInfo& pl) = 0;
};

class ShieldBuff : public Buff {
	public:
		ShieldBuff(PlayerInfo& pl);
		virtual void activate(PlayerInfo& pl);
};

class SpeedBuff : public Buff {
	public:
		SpeedBuff(PlayerInfo& pl);
		virtual void activate(PlayerInfo& pl);
};