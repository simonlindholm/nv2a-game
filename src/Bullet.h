#pragma once

#include <cstddef>
#include "Hitbox.h"
#include "util.h"

class Bullet {
	private:
		double dx, dy;
		double angle;
		Coord pos;
		size_t ownerPlayer;
		Hitbox hitbox;
	public:
		Bullet(const Coord& p, double ang, size_t owner);
		void move(unsigned int delay);
		Hitbox getHitbox() const;
		double getAngle() const;
		double getSpeed() const;
		double getDamage() const;
		size_t getOwner() const;
		Coord getPosition() const;
};
