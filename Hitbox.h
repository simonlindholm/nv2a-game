#pragma once

#include "util.h"

class Hitbox {
	private:
		// TODO: Support more than one circle
		Coord center;
		double radius;
	public:
		Hitbox(const Coord& pos, double rad);
		bool collidesWith(const Hitbox& other) const;
		void rotate(double angle);
		void move(const Coord& by);
};
