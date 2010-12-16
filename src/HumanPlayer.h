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
		bool initIgnKeys, noIgnoredKeys;
		bool ignoredKey[SDLK_LAST];
		std::list<int> ignKeys;

		void ignoreKeys();
		bool keyDown(Uint8* keyState, SDLKey key);
		void updateIgnoredKeys(Uint8* keyState);
	public:
		HumanPlayer();
		void startFrame(Uint8* keyState, Uint8 mouseState, int mouseX, int mouseY);
		void handleEvent(const SDL_Event& event);
		virtual Action move(const GameState& game, unsigned int delay);
		virtual void signalSpawn();
		void paint(const GameState& game, SDL_Surface* screen);
};
