#pragma once

#include <vector>
#include "shared_ptr.h"
#include "util.h"

class Circle;

// Abstract class symbolising shapes such as circles and rectangles, positioned
// anywhere in the plane and with any angle of rotation
class Shape {
	protected:
		Coord pos;
		Shape(const Coord& pos);

	public:
		virtual ~Shape() = 0;

		// Move the player by (by.x, by.y) - universally implemented in Shape
		void moveBy(const Coord& by);

		// Rotate the shape by angle around the origin
		virtual void rotate(double angle) = 0;

		// Clone the shape, returning a deep copy
		virtual shared_ptr<Shape> clone() const = 0;

		// Check if the shape collides with another one
		virtual bool collidesWith(const Shape& other) const = 0;

		// Functions to help with multiple dispatch for collision detection
		virtual bool collidesWithDisp(const Circle& other) const = 0;
};

class Circle : public Shape {
	// TODO: Add a custom small-object operator new, because we make a lot
	// of copies of this class
	private:
		double radius;
	public:
		Circle(const Coord& pos, double rad);
		virtual bool collidesWith(const Shape& other) const;
		virtual bool collidesWithDisp(const Circle& other) const;
		virtual void rotate(double angle);
		virtual shared_ptr<Shape> clone() const;
};

// Collection of shapes for use in collision detection
class Hitbox {
	// TODO: Implement bounding circles for faster testing
	private:
		std::vector<shared_ptr<Shape> > shapes;
	public:
		Hitbox();
		Hitbox(const Hitbox& other);
		Hitbox& operator=(const Hitbox& other);
		void swap(Hitbox& other);

		void add(shared_ptr<Shape> sh);
		bool collidesWith(const Hitbox& other) const;
		void rotate(double angle);
		void moveBy(const Coord& by);
};
