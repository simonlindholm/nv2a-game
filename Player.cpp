#include "Player.h"
#include "util.h"

Player::~Player() {}

Player::Player() {
	// XXX: Don't use hardcoded values
	hitbox.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 16)));
	rhitbox = hitbox;
	angle = 0;
	pos.x = pos.y = 0;
	speed = 0.2;
	hp = 100;
}

void Player::setAngle(double angle) {
	this->angle = angle;
	this->rhitbox.rawAssign(this->hitbox);
	this->rhitbox.rotate(angle);
}

double Player::getAngle() const {
	return this->angle;
}

void Player::moveTo(const Coord& to) {
	pos = to;
}

Coord Player::getPosition() const {
	return this->pos;
}

double Player::getSpeed() const {
	return this->speed;
}

void Player::setHP(int newHP) {
	this->hp = newHP;
}

int Player::getHP() const {
	return this->hp;
}

Hitbox Player::getHitbox() const {
	Hitbox ret = this->rhitbox;
	ret.moveBy(this->getPosition());
	return ret;
}
