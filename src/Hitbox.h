#pragma once

#include <vector>
#include "shared_ptr.h"
#include "util.h"

// Forward declarations of the shape subclasses
class Circle;
class Rectangle;

// Container class for all the collision detection functions, friend of all
// shape classes
class CollisionTests {
	public:
		static bool collRR(const Rectangle& a, const Rectangle& b);
		static bool collRC(const Rectangle& a, const Circle& b);
		static bool collCC(const Circle& a, const Circle& b);
};

// Abstract class symbolising shapes such as circles and rectangles, positioned
// anywhere in the plane and with any angle of rotation
class Shape {
	friend class CollisionTests;
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
		virtual bool collidesWithDisp(const Rectangle& other) const = 0;

		// Copy the object onto sh, which must be of the same type (no
		// typechecking is done, so use only if you are absolutely sure
		// about what the types are)
		virtual void rawAssign(const Shape& sh) = 0;
};

class Circle : public Shape {
	// TODO: Add a custom small-object operator new, because we make a lot
	// of copies of this class
	friend class CollisionTests;
	private:
		double radius;
	public:
		Circle(const Coord& pos, double rad);
		virtual bool collidesWith(const Shape& other) const;
		virtual bool collidesWithDisp(const Circle& other) const;
		virtual bool collidesWithDisp(const Rectangle& other) const;
		virtual void rotate(double angle);
		virtual shared_ptr<Shape> clone() const;
		virtual void rawAssign(const Shape& sh);
};

class Rectangle : public Shape {
	friend class CollisionTests;
	private:
		double w, h;
		double angle;
		bool axis;
	public:
		Rectangle(const Coord& pos, double width, double height);
		Rectangle(const Coord& pos, double width, double height, double angle);
		virtual bool collidesWith(const Shape& other) const;
		virtual bool collidesWithDisp(const Circle& other) const;
		virtual bool collidesWithDisp(const Rectangle& other) const;
		virtual void rotate(double angle);
		virtual shared_ptr<Shape> clone() const;
		virtual void rawAssign(const Shape& sh);
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

		// Make a copy of another hitbox, which must have a shape vector with
		// the same size and containing objects of the same type as this one
		// (no checking is done, so use only if you are absolutely sure that
		// this is the case)
		void rawAssign(const Hitbox& other);
};
