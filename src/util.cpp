#include <cstdlib>
#include "util.h"

Coord::Coord() : x(0), y(0) {}
Coord::Coord(double x, double y) : x(x), y(y) {}

unsigned int randTo(unsigned int to) {
	unsigned int ret = static_cast<unsigned int>(rand()/(RAND_MAX + 1.0)*to);
	if (ret >= to) --ret;
	return ret;
}

int randTo(int to) {
	return static_cast<int>(randTo(static_cast<unsigned int>(to)));
}

double randTo(double to) {
	return rand() / (RAND_MAX + 1.0) * to;
}

int randRange(int a, int b) {
	return a + randTo(static_cast<unsigned int>(b - a));
}

double randRange(double a, double b) {
	return a + randTo(b - a);
}
