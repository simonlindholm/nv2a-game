#include "Player.h"
#include "util.h"

Player::~Player() {}

Player::Player() {
	pos.x = pos.y = 200;
	angle = 0;
}

void Player::setAngle(double angle) {
	this->angle = angle;
}

double Player::getAngle() const {
	return this->angle;
}

void Player::moveBy(double x, double y) {
	pos.x += x;
	pos.y += y;
}

Coord Player::getPosition() const {
	return this->pos;
}
