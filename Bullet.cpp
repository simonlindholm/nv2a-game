#include <cmath>
#include "Bullet.h"

Bullet::Bullet(const Coord& p, double angle, size_t owner) {
	this->pos = p;
	this->angle = angle;
	this->ownerPlayer = owner;
}

double Bullet::getSpeed() const {
	return 1.0;
}

void Bullet::move(unsigned int delay) {
	double mov = delay * this->getSpeed();
	pos.x += mov * std::cos(this->angle);
	pos.y += mov * -std::sin(this->angle);

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
