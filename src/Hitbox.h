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
		static bool surfRR(const Rectangle& a, const Rectangle& b, Coord& out);
		static bool surfRC(const Rectangle& a, const Circle& b, Coord& out);
		static bool surfCC(const Circle& a, const Circle& b, Coord& out);
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

		// Find a unit vector directed from a point of 'this' which is nearly
		// colliding with 'other' against the surface involved in the near
		// collision, returning false if it is not yet implemented
		virtual bool collisionSurf(const Shape& other, Coord& out) const = 0;

		// Functions to help with multiple dispatch of the above function
		virtual bool collisionSurfDisp(const Circle& other, Coord& out) const = 0;
		virtual bool collisionSurfDisp(const Rectangle& other, Coord& out) const = 0;

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
		virtual bool collisionSurf(const Shape& other, Coord& out) const;
		virtual bool collisionSurfDisp(const Circle& other, Coord& out) const;
		virtual bool collisionSurfDisp(const Rectangle& other, Coord& out) const;
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
		virtual bool collisionSurf(const Shape& other, Coord& out) const;
		virtual bool collisionSurfDisp(const Circle& other, Coord& out) const;
		virtual bool collisionSurfDisp(const Rectangle& other, Coord& out) const;
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

		// Add a shape to the hitbox shape collection
		void add(shared_ptr<Shape> sh);

		// Check whether the hitbox collides with another hitbox
		bool collidesWith(const Hitbox& other) const;

		// Check whether the hitbox collides with another hitbox, returning
		// 0 for no collision, and non-zero for collision.
		// If implemented by the collision routine, 'out' will be modified
		// to contain a unit vector directed from where the hitbox was before
		// a relative movement of 'relm', towards the surface of the shape
		// collided with. In this case, the value 2 will be returned,
		// otherwise the return value will be 1.
		int collidesWithSurf(const Hitbox& other, Coord relm, Coord& out) const;

		// Rotate every shape around the origin
		void rotate(double angle);

		// Move every shape relatively
		void moveBy(const Coord& by);

		// Make a copy of another hitbox, which must have a shape vector with
		// the same size and containing objects of the same type as this one
		// (no checking is done, so use only if you are absolutely sure that
		// this is the case)
		void rawAssign(const Hitbox& other);
};
