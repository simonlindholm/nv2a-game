#include <cmath>
#include <stdexcept>
#include "Hitbox.h"
#include "mathutil.h"

// Helper functions
namespace{

Coord rotatePoint(const Coord& point, double angle) {
	if (fpEqual(point.x, 0, 0.1) && fpEqual(point.y, 0, 0.1)) return point;
	double abs = pyth(point.x, point.y);
	double pangle = std::atan2(-point.y, point.x);
	pangle += angle;
	Coord res;
	res.x = abs * std::cos(pangle);
	res.y = abs * -std::sin(pangle);
	return res;
}

// Helper function for the rectangle-rectangle collision detection routine,
// tests whether the point q is within any axis-aligned triangle with a right
// angle at position (px,py), width w and height h.
bool rrCorner(const Coord& q, double w, double h, double px, double py) {
	return w * std::abs(q.y - py) + h * std::abs(q.x - px) <= h * w;
}

}


// Collision detection functions
int CollisionTests::collRC(const Rectangle& a, const Circle& b, Coord* out) {
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

	if (c.pos.x >= 0 && c.pos.x <= a.w) {
		if (!out) return 1;
		out->x = 0;
		out->y = (c.pos.y < a.h / 2) ? -1 : 1;
		goto rot;
	}
	if (c.pos.y >= 0 && c.pos.y <= a.h) {
		if (!out) return 1;
		out->x = (c.pos.x < a.w / 2) ? -1 : 1;
		out->y = 0;
		goto rot;
	}

	{
		double xdif = c.pos.x;
		double ydif = c.pos.y;
		if (xdif > 0) xdif -= a.w;
		if (ydif > 0) ydif -= a.h;
		double dsq = xdif*xdif + ydif*ydif;
		if (dsq < c.radius*c.radius) {
			if (!out) return 1;
			double dist = std::sqrt(dsq);
			out->x = xdif/dist;
			out->y = ydif/dist;
			goto rot;
		}
	}
	return 0;

rot:
	// Return with 'out' rotated back into the original coordinate system
	if (!a.axis) *out = rotatePoint(*out, a.angle);
	return 2;
}

int CollisionTests::collCC(const Circle& a, const Circle& b, Coord* out) {
	// TODO: Calculate the surface normal vector

	double difx = a.pos.x - b.pos.x;
	double dify = a.pos.y - b.pos.y;
	double mdist = a.radius + b.radius;
	return (difx*difx + dify*dify < mdist*mdist) ? 1 : 0;
}

int CollisionTests::collRR(const Rectangle& a, const Rectangle& b, Coord* out) {
	// TODO: Calculate the surface normal vector

	if (b.axis) {
		if (a.axis) {
			// Both rectangles are axis-aligned, which makes the test trivial
			if (a.pos.x + a.w <= b.pos.x) return 0;
			if (b.pos.x + b.w <= a.pos.x) return 0;
			if (a.pos.y + a.h <= b.pos.y) return 0;
			if (b.pos.y + b.h <= a.pos.y) return 0;
			return 1;
		}
		else {
			// Order them so that a is the axis-aligned one
			bool ret = collRR(b, a, out);
			if (out) {
				out->x = -out->x;
				out->y = -out->y;
			}
			return ret;
		}
	}

	// As in the rectangle-circle test, create a copy of b and modify it so that
	// we can see the coordinate system as having 'a' at the origin, unrotated.
	Rectangle c = b;

	c.moveBy(Coord(-a.pos.x, -a.pos.y));
	if (!a.axis) c.rotate(-a.angle);

	double cang = std::cos(c.angle);
	double sang = std::sin(c.angle);

	// Switch the corner of reference for 'c', so that the angle in in [0, pi/2)
	while (c.angle >= m_pi/2) {
		c.pos.x += cang * c.w;
		c.pos.y -= sang * c.w;
		c.angle -= m_pi/2;
		std::swap(c.w, c.h);
		double osin = sang;
		sang = -cang;
		cang = osin;
	}

	// Do the actual test by checking whether the bottom-right corner of 'c' is
	// within the minkowski sum of 'a' and 'c', which now has a particular shape.
	double da = c.w * sang;
	double db = c.h * cang;
	double dc = c.w * cang;
	double dd = c.h * sang;

	Coord m = c.pos;
	m.x += dc + dd;
	m.y += db - da;
	double x1 = 0;
	double x2 = a.w + dc + dd;
	double y1 = -da;
	double y2 = a.h + db;

	if (m.x <= x1 || m.x >= x2) return 0;
	if (m.y <= y1 || m.y >= y2) return 0;

	if (rrCorner(m, dc, da, x1, y1)) return 0;
	if (rrCorner(m, dc, da, x2, y2)) return 0;
	if (rrCorner(m, dd, db, x2, y1)) return 0;
	if (rrCorner(m, dd, db, x1, y2)) return 0;

	return 1;
}


Shape::~Shape() {}

void Shape::moveBy(const Coord& by) {
	this->pos.x += by.x;
	this->pos.y += by.y;
}

Shape::Shape(const Coord& pos)
	: pos(pos)
{}


// Circle member functions
Circle::Circle(const Coord& pos, double rad)
	: Shape(pos), radius(rad)
{}

void Circle::rotate(double angle) {
	this->pos = rotatePoint(this->pos, angle);
}

int Circle::collidesWith(const Shape& other, Coord* out) const {
	return other.collidesWithDisp(*this, out);
}
int Circle::collidesWithDisp(const Circle& other, Coord* out) const {
	return CollisionTests::collCC(other, *this, out);
}
int Circle::collidesWithDisp(const Rectangle& other, Coord* out) const {
	return CollisionTests::collRC(other, *this, out);
}

shared_ptr<Shape> Circle::clone() const {
	return shared_ptr<Shape>(new Circle(*this));
}

void Circle::rawAssign(const Shape& sh) {
	*this = static_cast<const Circle&>(sh);
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

int Rectangle::collidesWith(const Shape& other, Coord* out) const {
	return other.collidesWithDisp(*this, out);
}
int Rectangle::collidesWithDisp(const Rectangle& other, Coord* out) const {
	return CollisionTests::collRR(other, *this, out);
}
int Rectangle::collidesWithDisp(const Circle& other, Coord* out) const {
	int ret = CollisionTests::collRC(*this, other, out);

	// We are checking a circle against a rectangle, but collRC gives the
	// surface normal vector as against the circle, so we need to negate
	// the vector
	if (ret == 2) {
		out->x = -out->x;
		out->y = -out->y;
	}

	return ret;
}

shared_ptr<Shape> Rectangle::clone() const {
	return shared_ptr<Shape>(new Rectangle(*this));
}

void Rectangle::rawAssign(const Shape& sh) {
	*this = static_cast<const Rectangle&>(sh);
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

int Hitbox::collidesWith(const Hitbox& other, Coord* out) const {
	size_t isz = this->shapes.size(), jsz = other.shapes.size();
	for (size_t i = 0; i < isz; ++i) {
		for (size_t j = 0; j < jsz; ++j) {
			if (int ret = this->shapes[i]->collidesWith(*other.shapes[j], out)) {
				return ret;
			}
		}
	}
	return 0;
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

void Hitbox::rawAssign(const Hitbox& other) {
	size_t sz = this->shapes.size();
	for (size_t i = 0; i < sz; ++i) {
		this->shapes[i]->rawAssign(*other.shapes[i]);
	}
}
