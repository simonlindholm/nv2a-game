#include <vector>
#include "StartFrame.h"
#include "ResourceCache.h"
#include "GameFrame.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "StupidAIPlayer.h"
#include "shared_ptr.h"
#include "util.h"

Frame* StartFrame::frame(SDL_Surface* screen, unsigned int delay) {
	// Program state begins here; pass state to GameFrame to run the game.
	// TODO: Change this to MenuFrame when that gets implemented.

	Level level;
	// TODO: Load levels from file.
	{
		level.bg = ResourceCache::get().getImg(StaticImages::LevelMap);

		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(0, 92), 75, 56)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(145, 0), 90, 77)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(248, 161), 149, 143)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(94, 204), 55, 56)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(0, 315), 75, 55)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(554, 0), 86, 100)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(554, 202), 86, 73)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(527, 370), 113, 5)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(257, 370), 191, 5)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(0, 370), 175, 5)));
		level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(470, 0), 5, 164)));

		level.startPoints.push_back(Coord(40, 230));
		level.startPoints.push_back(Coord(510, 35));
		level.startPoints.push_back(Coord(600, 425));

		level.spawnPoints = level.startPoints;
		level.startPoints.push_back(Coord(50, 45));
		level.startPoints.push_back(Coord(590, 135));
		level.startPoints.push_back(Coord(600, 320));
	}

	std::vector<shared_ptr<Player> > enemies;
	{
		std::vector<Coord> coords;
		coords.push_back(Coord(40, 230));
		coords.push_back(Coord(140, 120));
		coords.push_back(Coord(280, 115));
		coords.push_back(Coord(350, 70));
		coords.push_back(Coord(435, 140));
		coords.push_back(Coord(440, 310));
		coords.push_back(Coord(490, 385));
		coords.push_back(Coord(490, 425));
		coords.push_back(Coord(230, 425));
		coords.push_back(Coord(205, 320));
		coords.push_back(Coord(70, 285));
		shared_ptr<Player> enemy(new StupidAIPlayer(coords));
		enemies.push_back(enemy);
	}
	return new GameFrame(level, enemies);
}
