#include "Player.h"
#include "util.h"
#include "mathutil.h"

PlayerInfo::PlayerInfo() {
	// XXX: Don't use hardcoded values
	hitbox.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 16)));
	phitbox = rhitbox = hitbox;
	angle = 0;
	pos.x = pos.y = 0;
	hp = 100;
	resetStats();
	resetRegenTimer();
	resetShotTimer();
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

void PlayerInfo::setBuffTimer(unsigned int time) {
	resetStats();
	buffTimer.set(time);
}

void PlayerInfo::resetShotTimer() {
	shotTimer.set(200);
}

bool PlayerInfo::canShoot() {
	return shotTimer.isDone();
}

void PlayerInfo::step(unsigned int delay) {
	// Regenerate 15 HP per second after 6 seconds of not being hit
	if(hp < 100) {
		regenTimer.step(delay);
		if (regenTimer.isDone()) {
			second.step(delay);
			if (second.isDone()) {
				setHP(hp + 15);
				second.changeTime(1000);
			}
		}
	}

	// When the buff timer is done, stop the timer and call resetStats()
	if(buffTimer.isActive()) {
		buffTimer.step(delay);
		if(buffTimer.isDone()) {
			buffTimer.stop();
			resetStats();
		}
	}

	shotTimer.step(delay);
}


Player::~Player() {}

void Player::setInfo(const PlayerInfo* info) {
	this->info = info;
}
