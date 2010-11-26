#pragma once
#include "util.h"

// Small utility functions, structs, constants, etc.

// Definition of pi
const double m_pi = 3.14159265358979323846264338327;

// Reduce an angle to lie in the interval [0, 2pi)
double reduceAngle(double angle);

// Convert an angle from radians to degrees, rounding to nearest
int radToIntDeg(double rad);

// Calculate the hypotenuse in a right triangle with sides a and b, ie. sqrt(a*a+b*b)
double pyth(double a, double b);

// Rotate a given point by an angle given by a unit vector 'avec'
Coord rotatePointVec(const Coord& point, const Coord& avec);

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
