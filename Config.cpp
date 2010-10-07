#include <SDL/SDL.h>
#include <fstream>
#include <utility>
#include "Config.h"
#include "shared_ptr.h"

struct UserConfig {
	shared_ptr<int> fps;
	shared_ptr<std::pair<int, int> > dim;
	shared_ptr<bool> fullscreen;
};

// Read values from a user configuration (which can include unset values)
// into an actual configuration.
static void setFromUserConfig(Config& config, UserConfig& ucfg) {

	// Set fullscreen flag
	config.fullscreen = (ucfg.fullscreen ? *ucfg.fullscreen : false);

	// Set window dimensions
	if (ucfg.dim) {
		config.winHeight = ucfg.dim->first;
		config.winWidth = ucfg.dim->second;
	}
	else {
		if (config.fullscreen) {
			const SDL_VideoInfo* info = SDL_GetVideoInfo();
			config.winHeight = info->current_h;
			config.winWidth = info->current_w;
		}
		else {
			config.winHeight = 500;
			config.winWidth = 600;
		}
	}

	// Set minimum frame delay from FPS
	int fps = (ucfg.fps ? *ucfg.fps : 60);
	if (fps <= 0) {
		config.minDelay = 0;
	}
	else {
		config.minDelay = 1000 / fps;
	}
}

Config::Config() {
	// Initalize configuration with default values by setting it from
	// an empty user configuration
	UserConfig ucfg;
	setFromUserConfig(*this, ucfg);
}

// Load configuration from file
void Config::load(const char* filename) {
	// TODO: implement
}

Config& Config::get() {
	static Config singleton;
	return singleton;
}
