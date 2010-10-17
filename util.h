#pragma once

struct Coord {
	double x, y;
	Coord();
	Coord(double x, double y);
};

// Get a random number in the range [a, b)
int randRange(int a, int b);
