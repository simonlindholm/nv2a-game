#pragma once

// Singleton struct for holding configuration settings, such as window
// dimensions, FPS, keybinds, etc.
class Config {
	private:
		// Hide constructors from public view, since Config is a singleton
		Config();
		Config(const Config& other);

	public:
		// Publicly available properties
		int winWidth, winHeight;
		bool fullscreen;
		int minDelay;

		// Load configuration from file
		void load(const char* filename);

		// Return the config singleton (ie. the single Config instance that
		// exists in the program)
		static Config& get();
};
