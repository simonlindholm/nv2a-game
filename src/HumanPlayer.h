#pragma once
#include <SDL/SDL.h>
#include <list>
#include "Player.h"
#include "util.h"

// Implements a player controlled by the user
class HumanPlayer : public PlayerLogic {
	private:
		int vx, vy;
		Coord mouse;
		bool shooting;
		bool initIgnInput, noIgnoredInput;
		bool ignoredKey[SDLK_LAST];
		std::list<int> ignKeys;
		Uint8 ignMouse;
		SDL_Rect bgPos;

		void ignoreInput();
		bool keyDown(Uint8* keyState, SDLKey key);
		bool mouseButtonDown(Uint8 mouseState, int button);
		void updateIgnoredInput(Uint8* keyState, Uint8 mouseState);
	public:
		HumanPlayer();
		void startFrame(Uint8* keyState, Uint8 mouseState, int mouseX, int mouseY);
		void handleEvent(const SDL_Event& event);
		virtual Action move(const GameState& game, unsigned int delay);
		virtual void startGame(const GameState& game);
		virtual void signalSpawn();
		void paint(const GameState& game, SDL_Surface* screen);
};
