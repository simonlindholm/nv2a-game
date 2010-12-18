#include "Player.h"
#include "util.h"
#include "mathutil.h"

PlayerInfo::PlayerInfo() {
	hitbox.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 16)));
	phitbox = rhitbox = hitbox;
	angle = 0;
	pos.x = pos.y = 0;
	hp = 100;
	resetStats();
	resetRegenTimer();
	restartShotTimer();
}

void PlayerInfo::resetStats() {
	setDef(0);
	setSpeed(0.2);
}

double PlayerInfo::getAngle() const {
	return angle;
}

void PlayerInfo::setSpeed(double spd) {
	speed = spd;
}

double PlayerInfo::getSpeed() const {
	return speed;
}

void PlayerInfo::setAngle(double newAngle) {
	angle = reduceAngle(newAngle);
	rhitbox.rawAssign(hitbox);
	rhitbox.rotate(angle);
}

void PlayerInfo::moveTo(const Coord& to) {
	pos = to;
}

Coord PlayerInfo::getPosition() const {
	return pos;
}

void PlayerInfo::setHP(int newHP) {
	hp = newHP;

	if(hp > 100)
		hp = 100;
}

int PlayerInfo::getHP() const {
	return hp;
}

void PlayerInfo::setDef(int newDef) {
	defense = newDef;
}

int PlayerInfo::getDef() const {
	return defense;
}

const Hitbox& PlayerInfo::getHitbox() const {
	phitbox.rawAssign(rhitbox);
	phitbox.moveBy(this->getPosition());
	return phitbox;
}

void PlayerInfo::resetRegenTimer() {
	regenTimer.set(6000);
	second.set(0);
}

void PlayerInfo::restartShotTimer() {
	shotTimer.set(180);
}

bool PlayerInfo::canShoot() {
	return shotTimer.isDone();
}

void PlayerInfo::addBuff(shared_ptr<Buff> buff) {
	buffs.push_back(buff);
}

void PlayerInfo::recalculateBuffs() {
	resetStats();
	std::list<shared_ptr<Buff> >::iterator it = buffs.begin();
	while (it != buffs.end()) {
		(*it)->activate(*this);
		++it;
	}
}

void PlayerInfo::step(unsigned int delay) {
	// HP regeneration logic:
	// The player regenerates 15 HP per second after 6 seconds of not being hit.
	if (hp < 100) {
		regenTimer.step(delay);
		if (regenTimer.isDone()) {
			second.step(delay);
			if (second.isDone()) {
				setHP(hp + 15);
				second.changeTime(1000);
			}
		}
	}

    // Walk through the list of active buffs to see if any of them have ended.
    // If so, erase those from the list and recalculate the player state.
	std::list<shared_ptr<Buff> >::iterator it = buffs.begin();
	while (it != buffs.end())
	{
		bool del = false;
		shared_ptr<Buff> currentBuff = *it;
		currentBuff->step(delay);
		if (currentBuff->hasEnded()) {
			del = true;
		}

		if (del) {
			buffs.erase(it++);
			recalculateBuffs();
		}
		else {
			++it;
		}
	}

	shotTimer.step(delay);
}


PlayerLogic::~PlayerLogic() {}

void PlayerLogic::signalSpawn() {}

void PlayerLogic::initInfo(const PlayerInfo* info) {
	this->info = info;
}

Player::Player(shared_ptr<PlayerLogic> plogic)
	: info(), logic(plogic)
{
	logic->initInfo(&info);
}
