#include <cmath>
#include "Bullet.h"

Bullet::Bullet(const Coord& p, double angle, size_t owner) {
	this->pos = p;
	this->ownerPlayer = owner;
	this->dx = std::cos(angle);
	this->dy = -std::sin(angle);
	this->hitbox.add(shared_ptr<Shape>(new Circle(this->pos, 2.5)));
}

double Bullet::getSpeed() const {
	return 1.0;
}

double Bullet::getDamage() const {
	return 15.0;
}

void Bullet::move(unsigned int delay) {
	double mov = delay * this->getSpeed();
	this->pos.x += mov * this->dx;
	this->pos.y += mov * this->dy;
	this->hitbox.moveBy(Coord(mov * this->dx, mov * this->dy));
}

Hitbox Bullet::getHitbox() const {
	return this->hitbox;
}

size_t Bullet::getOwner() const {
	return this->ownerPlayer;
}

Coord Bullet::getPosition() const {
	return this->pos;
}
