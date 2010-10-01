#pragma once

// Struct for holding configuration settings, such as window dimensions, FPS,
// keybinds, etc.
struct Config {
	// XXX: Make this a singleton? (ie. a single global object, instead of main()
	// creating a Config and passing it to other classes as needed)
	int winWidth, winHeight;
	bool fullscreen;
	int minDelay;

	Config();
	void load(const char* filename);
};
