#include "Buff.h"
#include "Player.h"

Buff::Buff(unsigned int time) {
	buffTimer.set(time);
}

Buff::~Buff() {}

void Buff::step(unsigned int delay) {
	if (buffTimer.isActive()) {
		buffTimer.step(delay);
	}
}

bool Buff::hasEnded() const {
	return buffTimer.isDone();
}


ShieldBuff::ShieldBuff(PlayerInfo& pl) : Buff(5000) {
	this->activate(pl);
}

void ShieldBuff::activate(PlayerInfo &pl) {
	pl.setDef(10);
}

StaticImages::Id ShieldBuff::getIcon() const {
	return StaticImages::IconShield;
}


SpeedBuff::SpeedBuff(PlayerInfo& pl) : Buff(6000) {
	this->activate(pl);
}

void SpeedBuff::activate(PlayerInfo &pl) {
	pl.setSpeed(0.3);
}

StaticImages::Id SpeedBuff::getIcon() const {
	return StaticImages::IconSpeed;
}
