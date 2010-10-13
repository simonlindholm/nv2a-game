#include "Player.h"
#include "util.h"

Player::~Player() {}

Player::Player()
	: hitbox(Coord(0, 0), 10)
{
	// XXX: Don't use hardcoded values
	pos.x = pos.y = 200;
	angle = 0;
	speed = 0.2;
}

void Player::setAngle(double angle) {
	this->angle = angle;
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

Hitbox Player::getHitbox() const {
	Hitbox ret = this->hitbox;
	ret.rotate(this->getAngle());
	ret.move(this->getPosition());
	return ret;
}
