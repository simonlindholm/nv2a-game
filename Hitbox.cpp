#include <cmath>
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

// Circle functions
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
	double difx = this->pos.x - other.pos.x;
	double dify = this->pos.y - other.pos.y;
	double mdist = this->radius + other.radius;
	return (difx*difx + dify*dify < mdist*mdist);
}

shared_ptr<Shape> Circle::clone() const {
	return shared_ptr<Shape>(new Circle(*this));
}

// Rectangle functions
// TODO: Actually implement this

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
