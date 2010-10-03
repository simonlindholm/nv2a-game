#include <iostream>
#include <SDL/SDL.h>
#include "GameFrame.h"
#include "SDL_helpers.h"
#include "HumanPlayer.h"
#include "exceptions.h"
#include "shared_ptr.h"

GameFrame::GameFrame(shared_ptr<Player> enemy)
{
	player = shared_ptr<HumanPlayer>(new HumanPlayer);
	this->enemy = enemy;
}

Frame* GameFrame::frame(SDL_Surface* screen, unsigned int delay) {
	// (Game logic goes here.)

	// Pass keyboard and mouse data on to HumanPlayer
	SDL_PumpEvents();
	int mouseX, mouseY;
	Uint8* keyState = SDL_GetKeyState(0);
	Uint8 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	player->startFrame(keyState, mouseState, mouseX, mouseY);

	// Handle events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) throw ExitException();
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			// XXX: Add a menu on Escape?
			throw ExitException();
		}
		else {
			// Event wasn't used by the game screen, pass on the player interface
			player->handleEvent(event);
		}
	}

	// Move all players
	// XXX: Handle return values intelligently, and check hitboxes
	Player::Action ac = player->move(&gameState, delay);
	player->moveBy(ac.mx, ac.my);
	enemy->move(&gameState, delay);

	// Draw the player interface
	player->paint(&gameState, screen);
	return 0;
}
