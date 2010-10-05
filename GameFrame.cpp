#include <iostream>
#include <SDL/SDL.h>
#include "GameFrame.h"
#include "SDL_helpers.h"
#include "HumanPlayer.h"
#include "exceptions.h"
#include "shared_ptr.h"

GameFrame::GameFrame(const std::vector<shared_ptr<Player> >& enemies)
{
	player = shared_ptr<HumanPlayer>(new HumanPlayer);
	gameState.players = enemies;
	gameState.players.push_back(player);
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
			// TODO: Add a menu on Escape?
			throw ExitException();
		}
		else {
			// Event wasn't used by the game screen, pass on the player interface
			player->handleEvent(event);
		}
	}

	// Move all players
	for (unsigned int i = 0; i < gameState.players.size(); ++i) {
		shared_ptr<Player> p = gameState.players[i];
		Player::Action ac = p->move(gameState, delay);
		// TODO: Check hitboxes, and handle shooting correctly
		p->moveBy(ac.mx, ac.my);
	}

	// Draw the player interface
	player->paint(gameState, screen);
	return 0;
}
