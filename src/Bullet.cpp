#include <cmath>
#include "Bullet.h"

Bullet::Bullet(const Coord& p, double ang, size_t owner) {
	angle = ang;
	prevp = pos = p;
	ownerPlayer = owner;
	dx = std::cos(angle);
	dy = -std::sin(angle);
	hitboxLine = shared_ptr<LineSegment>(new LineSegment(pos, pos));
	hitbox.add(hitboxLine);
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
	prevp = pos;
	double mov = delay * this->getSpeed();
	pos.x += mov * dx;
	pos.y += mov * dy;
}

const Hitbox& Bullet::getHitbox() const {
	*hitboxLine = LineSegment(prevp, pos);
	return hitbox;
}

size_t Bullet::getOwner() const {
	return ownerPlayer;
}

Coord Bullet::getPosition() const {
	return pos;
}
