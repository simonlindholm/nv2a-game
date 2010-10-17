#include <cmath>
#include "Bullet.h"

Bullet::Bullet(const Coord& p, double angle, size_t owner) {
	this->pos = p;
	this->ownerPlayer = owner;
	this->dx = std::cos(angle);
	this->dy = -std::sin(angle);
}

double Bullet::getSpeed() const {
	return 1.0;
}

void Bullet::move(unsigned int delay) {
	double mov = delay * this->getSpeed();
	pos.x += mov * dx;
	pos.y += mov * dy;
}

Hitbox Bullet::getHitbox() const {
	return Hitbox(this->pos, 2.5);
}

size_t Bullet::getOwner() const {
	return this->ownerPlayer;
}

Coord Bullet::getPosition() const {
	return this->pos;
}
