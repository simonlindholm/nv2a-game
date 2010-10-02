#include <iostream>
#include <SDL/SDL.h>
#include "SDL_helpers.h"
#include "GameFrame.h"
#include "exceptions.h"
#include "shared_ptr.h"

GameFrame::GameFrame(shared_ptr<Player> enemy) {
	// XXX: Stub. Should also be extended to support multiple enemies
}

Frame* GameFrame::frame(SDL_Surface* screen, unsigned int delay) {
	// Main implementation of the game goes here

	// Basic event handling
	// TODO: Extend this to handle movement etc.
	Uint8* keyp=SDL_GetKeyState(NULL);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) throw ExitException();
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) throw ExitException();
		}
	}

	// TODO: Draw background and sprites here, instead of (as is happing right
	// now) just painting the screen red.
	SDL_FillRectLocked(screen, 255, 0, 0);
	return 0;
}
