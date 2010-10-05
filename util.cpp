#include <cmath>
#include "util.h"

Coord::Coord() : x(0), y(0) {}
Coord::Coord(double x,double y) : x(x), y(y) {}

double reduceAngle(double angle) {
	double ret = std::fmod(angle, 2*m_pi);
	if (ret < 0) ret += 2*m_pi;
	return ret;
}

template <class T>
bool fpEqualRelImpl(T a, T b, T epsilon) {
	T aa = std::fabs(a), ab = std::fabs(b), ad = std::fabs(a-b);
	T max = T(1);
	if (max < aa) max = aa;
	if (max < ab) max = ab;
	T relError = ad / max;
	return (relError <= epsilon);
}
bool fpEqualRel(float a, float b, float epsilon) {
	return fpEqualRelImpl(a, b, epsilon);
}
bool fpEqualRel(double a, double b, double epsilon) {
	return fpEqualRelImpl(a, b, epsilon);
}
bool fpEqualRel(long double a, long double b, long double epsilon) {
	return fpEqualRelImpl(a, b, epsilon);
}

template <class T>
bool fpEqualImpl(T a, T b, T epsilon) {
	return (fabs(a-b) <= epsilon);
}
bool fpEqual(float a, float b, float epsilon) {
	return fpEqualImpl(a, b, epsilon);
}
bool fpEqual(double a, double b, double epsilon) {
	return fpEqualImpl(a, b, epsilon);
}
bool fpEqual(long double a, long double b, long double epsilon) {
	return fpEqualImpl(a, b, epsilon);
}
