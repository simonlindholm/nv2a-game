#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <exception>
#include "exceptions.h"
#include "StartFrame.h"
#include "Config.h"

int main(int argc, char** argv) {
	int returnValue = 0;
	SDL_Surface* screen = 0;
	Frame* curFrame;
	bool hasSDL = false, hasMix = false;
	Config config;

	// Wrap all logic, including the game loop, in a try-catch; we quit (and print
	// an error message as appropriate) when we catch an exception.
	try {
		// Load configuration from file
		config.load("config.txt");

		// Initialize SDL and SDL_mixer
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
			throw GameException();
		}
		hasSDL = true;

		if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024) == -1) {
			throw GameException();
		}
		hasMix = true;

		// Set up the window
		SDL_WM_SetCaption("The NV2A Project", "The NV2A Project");

		Uint32 videoOptions = SDL_HWSURFACE | SDL_DOUBLEBUF;
		if (config.fullscreen) videoOptions |= SDL_FULLSCREEN;

		screen = SDL_SetVideoMode(config.winWidth, config.winHeight, 32, videoOptions);
		if (!screen){
			throw GameException();
		}
		SDL_FillRect(screen, 0, 0x000000);

		// Begin in a 'begin state' given by StartFrame
		curFrame = new StartFrame;

		Uint32 ltime = SDL_GetTicks();

		// Main game loop
		for(;;) {
			Uint32 ctime = SDL_GetTicks();
			int sdelay = ctime - ltime;
			while (sdelay < 0) {
				// Correct timer errors (not sure if this can realistically happen)
				SDL_Delay(1);
				ctime = SDL_GetTicks();
				sdelay = ctime - ltime;
			}
			unsigned int delay = (unsigned int)sdelay;

			// Draw the current frame
			Frame* nextFrame = curFrame->frame(screen, delay);
			while (nextFrame) {
				delete curFrame;
				curFrame = nextFrame;
				nextFrame = curFrame->frame(screen, 0);
			}
			SDL_Flip(screen);

			// Delay by some time so that the CPU doesn't max out, and match this
			// duration to the specified FPS.
			int sleep = (ctime + config.minDelay) - SDL_GetTicks();
			if (sleep > 0) SDL_Delay(sleep);

			ltime = ctime;
		}
	}
	catch (const ExitException& e) {
		// Exit nicely
	}
	catch (const std::exception& e) {
		// An error was raised, print it and return 1 from program
		std::cerr << e.what() << std::endl;
		returnValue = 1;
	}

	// Clean up
	if (curFrame) delete curFrame;
	if (hasMix) Mix_CloseAudio();
	if (hasSDL) SDL_Quit();

	return returnValue;
}
