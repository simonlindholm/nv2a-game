#pragma once

#include <cstddef>
#include "Hitbox.h"
#include "util.h"

class Bullet {
	private:
		double dx, dy;
		Coord pos;
		size_t ownerPlayer;
	public:
		Bullet(const Coord& p, double angle, size_t owner);
		void move(unsigned int delay);
		Hitbox getHitbox() const;
		double getSpeed() const;
		size_t getOwner() const;
		Coord getPosition() const;
};
