#include <cmath>
#include <stdexcept>
#include "Hitbox.h"
#include "mathutil.h"

// Container class for all the collision detection functions, friend of all
// shape classes
class CollisionTests {
	public:
		static bool collRR(const Rectangle& a, const Rectangle& b);
		static bool collRC(const Rectangle& a, const Circle& b);
		static bool collCC(const Circle& a, const Circle& b);
		static bool collRL(const Rectangle& a, const LineSegment& b);
		static bool collCL(const Circle& a, const LineSegment& b);
		static bool collLL(const LineSegment& a, const LineSegment& b);
		static bool surfRR(const Rectangle& a, const Rectangle& b, Coord& out);
		static bool surfRC(const Rectangle& a, const Circle& b, Coord& out);
		static bool surfCC(const Circle& a, const Circle& b, Coord& out);
};

// Helper functions
namespace{

Coord rotatePoint(const Coord& point, double angle) {
	// Note: Not sure if this works correctly in all cases, but it seems so.
	Coord v;
	v.x = std::cos(angle);
	v.y = std::sin(angle);
	return rotatePointVec(point, v);
}

// Helper function for the rectangle-rectangle collision detection routine,
// tests whether the point q is within any axis-aligned triangle with a right
// angle at position (px,py), width w and height h.
bool rrCorner(const Coord& q, double w, double h, double px, double py) {
	return w * std::abs(q.y - py) + h * std::abs(q.x - px) <= h * w;
}

// Test whether the line segment p1~p2 intersects (x1,0)~(x2,0),
// assuming x1 <= x2
bool collRLine(double x1, double x2, Coord p1, Coord p2) {
	// Test whether p1~p2 intersects the real line
	if (p1.y > p2.y) std::swap(p1, p2);
	if (p1.y > 0 || p2.y < 0) return false;

	if (p1.y == p2.y) {
		// Handle the exceptional case where the two line segments are parallel
		if (p1.x > p2.x) std::swap(p1.x, p2.x);
		return (p1.x <= x2 && x1 <= p2.x);
	}

	// Calculate the intersection point
	double xdif = p2.x - p1.x, ydif = p2.y - p1.y;
	double x = p1.x - p1.y * xdif / ydif;
	return (x1 <= x && x <= x2);
}

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

	if (c.pos.x >= 0 && c.pos.x <= a.w) return true;
	if (c.pos.y >= 0 && c.pos.y <= a.h) return true;

	double xdif = c.pos.x;
	double ydif = c.pos.y;
	if (xdif > 0) xdif -= a.w;
	if (ydif > 0) ydif -= a.h;
	double dsq = xdif*xdif + ydif*ydif;
	return (dsq < c.radius*c.radius);
}

bool CollisionTests::collCC(const Circle& a, const Circle& b) {
	double difx = a.pos.x - b.pos.x;
	double dify = a.pos.y - b.pos.y;
	double mdist = a.radius + b.radius;
	return (difx*difx + dify*dify < mdist*mdist);
}

bool CollisionTests::collRR(const Rectangle& a, const Rectangle& b) {
	if (b.axis) {
		if (a.axis) {
			// Both rectangles are axis-aligned, which makes the test trivial
			if (a.pos.x + a.w <= b.pos.x) return false;
			if (b.pos.x + b.w <= a.pos.x) return false;
			if (a.pos.y + a.h <= b.pos.y) return false;
			if (b.pos.y + b.h <= a.pos.y) return false;
			return true;
		}
		else {
			// Order them so that a is the axis-aligned one
			return collRR(b, a);
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

	if (m.x <= x1 || m.x >= x2) return false;
	if (m.y <= y1 || m.y >= y2) return false;

	if (rrCorner(m, dc, da, x1, y1)) return false;
	if (rrCorner(m, dc, da, x2, y2)) return false;
	if (rrCorner(m, dd, db, x2, y1)) return false;
	if (rrCorner(m, dd, db, x1, y2)) return false;

	return true;
}

bool CollisionTests::collRL(const Rectangle& a, const LineSegment& b) {
	// Make a copy of the line segment so we can modify it
	LineSegment ls = b;

	// Transform the coordinate system so that the rectangle is axis-aligned
	// and is located at the origin
	ls.pos.x -= a.pos.x;
	ls.pos.y -= a.pos.y;
	ls.p2.x -= a.pos.x;
	ls.p2.y -= a.pos.y;
	if (!a.axis) {
		ls.pos = rotatePoint(ls.pos, -a.angle);
		ls.p2 = rotatePoint(ls.p2, -a.angle);
	}

	// If one of the points is within the rectangle then there is a collision
	if (ls.pos.x >= 0 && ls.pos.x <= a.w && ls.pos.y >= 0 && ls.pos.y <= a.h)
		return true;

	// Otherwise, a collision must involve the line segment intersecting one
	// of the sides, and since these sides are axis-aligned we can use
	// collRLine for this, with displaced/reflected endpoints
	if (collRLine(0, a.w, ls.pos, ls.p2)) return true;

	ls.pos.y -= a.h;
	ls.p2.y -= a.h;
	if (collRLine(0, a.w, ls.pos, ls.p2)) return true;
	ls.pos.y += a.h;
	ls.p2.y += a.h;

	// Reflect everything around y=x, ie. swap x and y coordinates (this does
	// not change whether the shapes collide, but makes it possible to use
	// collRLine despite it operating only with horizontal lines)
	std::swap(ls.pos.x, ls.pos.y);
	std::swap(ls.p2.x, ls.p2.y);

	if (collRLine(0, a.h, ls.pos, ls.p2)) return true;

	ls.pos.y -= a.w;
	ls.p2.y -= a.w;
	if (collRLine(0, a.h, ls.pos, ls.p2)) return true;
	
	return false;
}

bool CollisionTests::collCL(const Circle& a, const LineSegment& b) {
	Circle c = a;
	c.pos.x -= b.pos.x;
	c.pos.y -= b.pos.y;
	Coord oth = b.p2;
	oth.x -= b.pos.x;
	oth.y -= b.pos.y;
	// TODO: Keep an angle in LineSegment, so this isn't needed
	double ang = atan2(oth.y, oth.x);
	c.pos = rotatePoint(c.pos, ang);
	oth = rotatePoint(oth, ang);

	if (c.pos.y + c.radius < 0 || c.pos.y - c.radius > 0) return false;
	double xsqmax = c.radius * c.radius - c.pos.y * c.pos.y;
	if (c.pos.x > 0)
		return (c.pos.x * c.pos.x <= xsqmax);
	double xd2 = c.pos.x + oth.x;
	if (xd2 < 0)
		return (xd2 * xd2 <= xsqmax);
	return true;
}

bool CollisionTests::collLL(const LineSegment&, const LineSegment&) {
	// TODO: Implement.
	return false;
}

// Functions for finding a unit vector directed against the relevant surface
bool CollisionTests::surfRC(const Rectangle& a, const Circle& b, Coord& out) {
	// Create a copy of the circle to allow moving it, which makes
	// the algorithm simpler
	Circle c = b;

	// Transform the coordinate system so that the rectangle is located at
	// the origin, and is axis-aligned
	c.moveBy(Coord(-a.pos.x, -a.pos.y));
	if (!a.axis) c.rotate(-a.angle);

	if (c.pos.x >= 0 && c.pos.x <= a.w) {
		out.x = 0;
		out.y = (c.pos.y < a.h / 2) ? -1 : 1;
	}
	else if (c.pos.y >= 0 && c.pos.y <= a.h) {
		out.x = (c.pos.x < a.w / 2) ? -1 : 1;
		out.y = 0;
	}
	else {
		double xdif = c.pos.x;
		double ydif = c.pos.y;
		if (xdif > 0) xdif -= a.w;
		if (ydif > 0) ydif -= a.h;
		double dist = pyth(xdif, ydif);
		out.x = xdif/dist;
		out.y = ydif/dist;
	}

	// Return with 'out' rotated back into the original coordinate system
	if (!a.axis) out = rotatePoint(out, a.angle);

	return true;
}

bool CollisionTests::surfRR(const Rectangle&, const Rectangle&, Coord&) {
	// TODO: Implement some day, maybe.
	return false;
}

bool CollisionTests::surfCC(const Circle& a, const Circle& b, Coord& out) {
	double xdif = b.pos.x - a.pos.x;
	double ydif = b.pos.y - a.pos.y;
	double dist = pyth(xdif, ydif);
	out.x = xdif/dist;
	out.y = ydif/dist;
	return true;
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

bool Circle::collidesWith(const Shape& other) const {
	return other.collidesWithDisp(*this);
}
bool Circle::collidesWithDisp(const Circle& other) const {
	return CollisionTests::collCC(other, *this);
}
bool Circle::collidesWithDisp(const Rectangle& other) const {
	return CollisionTests::collRC(other, *this);
}
bool Circle::collidesWithDisp(const LineSegment& other) const {
	return CollisionTests::collCL(*this, other);
}

bool Circle::collisionSurf(const Shape& other, Coord& out) const {
	return other.collisionSurfDisp(*this, out);
}
bool Circle::collisionSurfDisp(const Rectangle& other, Coord& out) const {
	return CollisionTests::surfRC(other, *this, out);
}
bool Circle::collisionSurfDisp(const Circle& other, Coord& out) const {
	return CollisionTests::surfCC(other, *this, out);
}
bool Circle::collisionSurfDisp(const LineSegment&, Coord&) const {
	return false;
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

bool Rectangle::collidesWith(const Shape& other) const {
	return other.collidesWithDisp(*this);
}
bool Rectangle::collidesWithDisp(const Rectangle& other) const {
	return CollisionTests::collRR(other, *this);
}
bool Rectangle::collidesWithDisp(const Circle& other) const {
	return CollisionTests::collRC(*this, other);
}
bool Rectangle::collidesWithDisp(const LineSegment& other) const {
	return CollisionTests::collRL(*this, other);
}

bool Rectangle::collisionSurf(const Shape& other, Coord& out) const {
	return other.collisionSurfDisp(*this, out);
}
bool Rectangle::collisionSurfDisp(const Rectangle& other, Coord& out) const {
	return CollisionTests::surfRR(other, *this, out);
}
bool Rectangle::collisionSurfDisp(const Circle& other, Coord& out) const {
	bool ret = CollisionTests::surfRC(*this, other, out);

	// We are checking a circle against a rectangle, but surfRC gives the
	// surface normal vector as against the circle, so we need to negate
	// the vector
	if (ret) {
		out.x = -out.x;
		out.y = -out.y;
	}

	return ret;
}
bool Rectangle::collisionSurfDisp(const LineSegment&, Coord&) const {
	return false;
}

shared_ptr<Shape> Rectangle::clone() const {
	return shared_ptr<Shape>(new Rectangle(*this));
}

void Rectangle::rawAssign(const Shape& sh) {
	*this = static_cast<const Rectangle&>(sh);
}


// LineSegment member functions
LineSegment::LineSegment(const Coord& a, const Coord& b)
	: Shape(a), p2(b)
{}

void LineSegment::rotate(double angle) {
	pos = rotatePoint(pos, angle);
	p2 = rotatePoint(p2, angle);
}

bool LineSegment::collidesWith(const Shape& other) const {
	return other.collidesWithDisp(*this);
}
bool LineSegment::collidesWithDisp(const Circle& other) const {
	return CollisionTests::collCL(other, *this);
}
bool LineSegment::collidesWithDisp(const Rectangle& other) const {
	return CollisionTests::collRL(other, *this);
}
bool LineSegment::collidesWithDisp(const LineSegment& other) const {
	return CollisionTests::collLL(other, *this);
}

bool LineSegment::collisionSurf(const Shape&, Coord&) const {
	return false;
}
bool LineSegment::collisionSurfDisp(const Rectangle&, Coord&) const {
	return false;
}
bool LineSegment::collisionSurfDisp(const Circle&, Coord&) const {
	return false;
}
bool LineSegment::collisionSurfDisp(const LineSegment&, Coord&) const {
	return false;
}

shared_ptr<Shape> LineSegment::clone() const {
	return shared_ptr<Shape>(new LineSegment(*this));
}

void LineSegment::rawAssign(const Shape& sh) {
	*this = static_cast<const LineSegment&>(sh);
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
			if (this->shapes[i]->collidesWith(*other.shapes[j])) {
				return true;
			}
		}
	}
	return false;
}

int Hitbox::collidesWithSurf(const Hitbox& other, Coord relm, Coord& out) const {
	size_t isz = this->shapes.size(), jsz = other.shapes.size();
	for (size_t i = 0; i < isz; ++i) {
		for (size_t j = 0; j < jsz; ++j) {
			if (this->shapes[i]->collidesWith(*other.shapes[j])) {
				shared_ptr<Shape> tsh = this->shapes[i]->clone();
				relm.x = -relm.x;
				relm.y = -relm.y;
				tsh->moveBy(relm);
				bool hasSurf = tsh->collisionSurf(*other.shapes[j], out);
				return (hasSurf ? 2 : 1);
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
