#pragma once
#include <cstddef>
#include "Hitbox.h"
#include "util.h"

class Bullet {
	private:
		double dx, dy;
		double angle;
		Coord pos, prevp;
		size_t ownerPlayer;
		shared_ptr<LineSegment> hitboxLine;
		Hitbox hitbox;
	public:
		Bullet(const Coord& p, double ang, size_t owner);
		void move(unsigned int delay);
		const Hitbox& getHitbox() const;
		double getAngle() const;
		double getSpeed() const;
		double getDamage() const;
		size_t getOwner() const;
		Coord getPosition() const;
};
