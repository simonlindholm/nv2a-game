#include <cmath>
#include <stdexcept>
#include "Hitbox.h"
#include "mathutil.h"

Shape::~Shape() {}

void Shape::moveBy(const Coord& by) {
	this->pos.x += by.x;
	this->pos.y += by.y;
}

Shape::Shape(const Coord& pos)
	: pos(pos)
{}

static Coord rotatePoint(const Coord& point, double angle) {
	if (fpEqual(point.x, 0, 0.1) && fpEqual(point.y, 0, 0.1)) return point;
	double abs = pyth(point.x, point.y);
	double pangle = std::atan2(-point.y, point.x);
	pangle += angle;
	Coord res;
	res.x = abs * std::cos(pangle);
	res.y = abs * -std::sin(pangle);
	return res;
}


// Collision detection functions
bool CollisionTests::collRC(const Rectangle& a, const Circle& b) {
	// Create a copy of the circle to allow moving it, which makes
	// the algorithm simpler
	Circle c = b;

	// Transform the coordinate system so that the rectangle is located at
	// the origin, and is axis-aligned
	c.moveBy(Coord(-a.pos.x, -a.pos.y));
	if (!a.axis) c.rotate(-a.angle);

	// Now, check whether the circle's center is within the minkowski sum
	// of the two shapes
	if (c.pos.x <= -c.radius || c.pos.x >= a.w + c.radius) return false;
	if (c.pos.y <= -c.radius || c.pos.y >= a.h + c.radius) return false;

	if (c.pos.x >= 0 && c.pos.x <= a.w &&
		c.pos.y > -c.radius && c.pos.y < a.h + c.radius) return true;
	if (c.pos.y >= 0 && c.pos.y <= a.h &&
		c.pos.x > -c.radius && c.pos.x < a.w + c.radius) return true;

	double xdif = std::min(std::abs(c.pos.x), std::abs(c.pos.x - a.w));
	double ydif = std::min(std::abs(c.pos.y), std::abs(c.pos.y - a.h));
	return (xdif*xdif + ydif*ydif < c.radius*c.radius);
}

bool CollisionTests::collCC(const Circle& a, const Circle& b) {
	double difx = a.pos.x - b.pos.x;
	double dify = a.pos.y - b.pos.y;
	double mdist = a.radius + b.radius;
	return (difx*difx + dify*dify < mdist*mdist);
}

bool CollisionTests::collRR(const Rectangle& a, const Rectangle& b) {
	// TODO: Implement this.
	throw std::logic_error
		("Rectangle to rectangle collision detection not yet implemented.");
}


// Circle member functions
Circle::Circle(const Coord& pos, double rad)
	: Shape(pos), radius(rad)
{}

void Circle::rotate(double angle) {
	this->pos = rotatePoint(this->pos, angle);
}

bool Circle::collidesWith(const Shape& other) const {
	return other.collidesWithDisp(*this);
}
bool Circle::collidesWithDisp(const Circle& other) const {
	return CollisionTests::collCC(other, *this);
}
bool Circle::collidesWithDisp(const Rectangle& other) const {
	return CollisionTests::collRC(other, *this);
}

shared_ptr<Shape> Circle::clone() const {
	return shared_ptr<Shape>(new Circle(*this));
}


// Rectangle member functions
Rectangle::Rectangle(const Coord& pos, double width, double height)
	: Shape(pos), w(width), h(height), angle(0), axis(true)
{}

Rectangle::Rectangle(const Coord& pos, double width, double height, double angle)
	: Shape(pos), w(width), h(height), angle(angle), axis(false)
{}

void Rectangle::rotate(double angle) {
	this->pos = rotatePoint(this->pos, angle);
	this->angle = reduceAngle(this->angle + angle);
	this->axis = false;
}

bool Rectangle::collidesWith(const Shape& other) const {
	return other.collidesWithDisp(*this);
}
bool Rectangle::collidesWithDisp(const Rectangle& other) const {
	return CollisionTests::collRR(*this, other);
}
bool Rectangle::collidesWithDisp(const Circle& other) const {
	return CollisionTests::collRC(*this, other);
}

shared_ptr<Shape> Rectangle::clone() const {
	return shared_ptr<Shape>(new Rectangle(*this));
}


// Hitbox functions
Hitbox::Hitbox() {}
Hitbox::Hitbox(const Hitbox& other) {
	this->shapes.resize(other.shapes.size());
	for (size_t i = 0; i < other.shapes.size(); ++i) {
		this->shapes[i] = other.shapes[i]->clone();
	}
}
Hitbox& Hitbox::operator=(const Hitbox& other) {
	Hitbox temp(other);
	swap(temp);
	return *this;
}
void Hitbox::swap(Hitbox& other) {
	this->shapes.swap(other.shapes);
}

void Hitbox::add(shared_ptr<Shape> sh) {
	this->shapes.push_back(sh);
}

bool Hitbox::collidesWith(const Hitbox& other) const {
	size_t isz = this->shapes.size(), jsz = other.shapes.size();
	for (size_t i = 0; i < isz; ++i) {
		for (size_t j = 0; j < jsz; ++j) {
			if (this->shapes[i]->collidesWith(*other.shapes[j])) return true;
		}
	}
	return false;
}

void Hitbox::rotate(double angle) {
	size_t sz = this->shapes.size();
	for (size_t i = 0; i < sz; ++i) {
		this->shapes[i]->rotate(angle);
	}
}

void Hitbox::moveBy(const Coord& by) {
	size_t sz = this->shapes.size();
	for (size_t i = 0; i < sz; ++i) {
		this->shapes[i]->moveBy(by);
	}
}
