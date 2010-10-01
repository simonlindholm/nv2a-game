#include "Config.h"
#include <fstream>

Config::Config() {
	// Initialize config with default values
	fullscreen = false;
	winWidth = 600;
	winHeight = 500;
	minDelay = 1000/60; // Cap framerate to at most 60 FPS
}

// Load configuration from file
void Config::load(const char* filename) {
	// TODO: implement
}
