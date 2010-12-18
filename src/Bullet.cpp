#include <cmath>
#include "Bullet.h"

Bullet::Bullet(const Coord& p, double ang, size_t owner) {
	angle = ang;
	pos = p;
	ownerPlayer = owner;
	dx = std::cos(angle);
	dy = -std::sin(angle);
	hitbox.add(shared_ptr<Shape>(new Circle(this->pos, 2.5)));
}

double Bullet::getSpeed() const {
	return 1.0;
}

double Bullet::getDamage() const {
	return 15.0;
}

double Bullet::getAngle() const {
	return angle;
}

void Bullet::move(unsigned int delay) {
	double mov = delay * this->getSpeed();
	pos.x += mov * dx;
	pos.y += mov * dy;
	hitbox.moveBy(Coord(mov * dx, mov * dy));
}

Hitbox Bullet::getHitbox() const {
	return hitbox;
}

size_t Bullet::getOwner() const {
	return ownerPlayer;
}

Coord Bullet::getPosition() const {
	return pos;
}
