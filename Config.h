#pragma once
#include "exceptions.h"

struct Config {
	int winWidth, winHeight;
	bool fullscreen;
	int minDelay;

	Config();
	void load(const char* filename);
};
