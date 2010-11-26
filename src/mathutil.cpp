#include <cmath>
#include "mathutil.h"

double reduceAngle(double angle) {
	double ret = std::fmod(angle, 2*m_pi);
	if (ret < 0) ret += 2*m_pi;
	return ret;
}

int radToIntDeg(double rad) {
	double deg = rad * 180 / m_pi;
	int ideg = static_cast<int>(deg + 0.5);
	ideg %= 360;
	if (ideg < 0) ideg += 360;
	return ideg;
}

double pyth(double a, double b) {
	return std::sqrt(a*a + b*b);
}

Coord rotatePointVec(const Coord& point, const Coord& avec) {
	Coord res;
	res.x = point.x*avec.x - point.y*avec.y;
	res.y = point.y*avec.x + point.x*avec.y;
	return res;
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
