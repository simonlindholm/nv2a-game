#include <cmath>
#include "Hitbox.h"
#include "mathutil.h"

Hitbox::Hitbox(const Coord& pos, double rad) {
	this->radius = rad;
	this->center = pos;
}

static Coord rotatePoint(const Coord& point, double angle) {
	if (fpEqual(point.x, 0, 0.1) && fpEqual(point.y, 0, 0.1)) return point;
	double abs = std::sqrt(point.x*point.x + point.y*point.y);
	double pangle = std::atan2(-point.y, point.x);
	pangle += angle;
	Coord res;
	res.x = abs * std::cos(pangle);
	res.y = abs * -std::sin(pangle);
	return res;
}

void Hitbox::rotate(double angle) {
	this->center = rotatePoint(this->center, angle);
}

void Hitbox::move(const Coord& by) {
	this->center.x += by.x;
	this->center.y += by.y;
}

bool Hitbox::collidesWith(const Hitbox& other) const {
	double difx = this->center.x - other.center.x;
	double dify = this->center.y - other.center.y;
	double mdist = this->radius + other.radius;
	return (difx*difx + dify*dify < mdist*mdist);
}
