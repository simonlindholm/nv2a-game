#pragma once

struct Coord {
	double x, y;
	Coord();
	Coord(double x, double y);
};

// Get a random number in the range [0, to)
unsigned int randTo(unsigned int to);
int randTo(int to);
double randTo(double to);

// Get a random number in the range [a, b)
int randRange(int a, int b);
double randRange(double a, double b);
