#include "Buff.h"
#include "Player.h"

Buff::~Buff() {}

void Buff::setBuffTimer(unsigned int time) {
	buffTimer.set(time);
}

void Buff::step(unsigned int delay) {
	if (buffTimer.isActive()) {
		buffTimer.step(delay);
	}
}

bool Buff::hasEnded() const {
	return buffTimer.isDone();
}


ShieldBuff::ShieldBuff(PlayerInfo& pl) {
	this->activate(pl);
	this->setBuffTimer(5000);
}

void ShieldBuff::activate(PlayerInfo &pl) {
	pl.setDef(10);
}


SpeedBuff::SpeedBuff(PlayerInfo& pl) {
	this->activate(pl);
	this->setBuffTimer(6000);
}

void SpeedBuff::activate(PlayerInfo &pl) {
	pl.setSpeed(0.3);
}
