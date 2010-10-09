#include "Player.h"
#include "util.h"

Player::~Player() {}

Player::Player() {
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

void Player::moveTo(double x, double y) {
	pos.x = x;
	pos.y = y;
}

Coord Player::getPosition() const {
	return this->pos;
}

double Player::getSpeed() const {
	return this->speed;
}
