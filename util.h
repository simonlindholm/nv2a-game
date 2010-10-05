#pragma once

// Small utility functions, structs, constants, etc.

// Definition of pi
const double m_pi = 3.14159265358979323846264338327;

// A coordinate
struct Coord {
	double x, y;
	Coord();
	Coord(double x, double y);
};

double reduceAngle(double angle);
// Floating point equality tests
// (Note: Floating point comparisons are inexact, so instead of comparing
// equality we check whether the difference is sufficiently small.)

// Check whether a and b differ by at most epsilon
bool fpEqual(float a, float b, float epsilon);
bool fpEqual(double a, double b, double epsilon);
bool fpEqual(long double a, long double b, long double epsilon);

// Check whether a and b have a relative error of at most epsilon, compared to
// the greatest of |a|, |b| and 1.
bool fpEqualRel(float a, float b, float epsilon);
bool fpEqualRel(double a, double b, double epsilon);
bool fpEqualRel(long double a, long double b, long double epsilon);
