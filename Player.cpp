#include "Player.h"
#include "util.h"
#include "mathutil.h"

PlayerInfo::PlayerInfo() {
	// XXX: Don't use hardcoded values
	hitbox.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 16)));
	phitbox = rhitbox = hitbox;
	angle = 0;
	pos.x = pos.y = 0;
	speed = 0.2;
	hp = 100;
	regenTimer.set(5000);
	second.set(1000);
}

double PlayerInfo::getAngle() const {
	return angle;
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

const Hitbox& PlayerInfo::getHitbox() const {
	phitbox.rawAssign(rhitbox);
	phitbox.moveBy(this->getPosition());
	return phitbox;
}

void PlayerInfo::resetRegenTimer() {
	regenTimer.set(5000);
	second.set(1000);
}

void PlayerInfo::regenTimerLogic(unsigned int delay) {
	regenTimer.step(delay);
	if (regenTimer.isDone()) {
		second.step(delay);
		if (second.isDone()) {
			setHP(hp + 15);
			second.changeTime(1000);
		}
	}
}


Player::~Player() {}

void Player::setInfo(const PlayerInfo* info) {
	this->info = info;
}
