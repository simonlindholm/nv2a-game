#include <cstdlib>
#include "util.h"

Coord::Coord() : x(0), y(0) {}
Coord::Coord(double x,double y) : x(x), y(y) {}

int randRange(int a, int b) {
	return a + rand() % (b-a);
}
