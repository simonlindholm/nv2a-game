#include <vector>
#include "StartFrame.h"
#include "GameFrame.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "StupidAIPlayer.h"
#include "shared_ptr.h"
#include "util.h"

Frame* StartFrame::frame(SDL_Surface* screen, unsigned int delay) {
	// Program state begins here; pass state to GameFrame to run the game.
	// TODO: Change this to MenuFrame when that gets implemented.

	std::vector<shared_ptr<Player> > enemies;
	{
		std::vector<Coord> coords;
		coords.push_back(Coord(30, 30));
		coords.push_back(Coord(300, 100));
		coords.push_back(Coord(100, 300));
		shared_ptr<Player> enemy(new StupidAIPlayer(coords));
		enemy->moveTo(30, 30); // XXX: Hack to start enemy at (30, 30)
		enemies.push_back(enemy);
	}
	return new GameFrame(enemies);
}
