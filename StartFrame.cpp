#include "StartFrame.h"
#include "GameFrame.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "StupidAIPlayer.h"
#include "shared_ptr.h"

Frame* StartFrame::frame(SDL_Surface* screen, unsigned int delay) {
	// Program state begins here; pass state to GameFrame to run the game.
	// XXX: Change this to MenuFrame when that gets implemented.
	shared_ptr<Player> enemy(new StupidAIPlayer);
	return new GameFrame(enemy);
}
