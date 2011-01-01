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

		level.startingPoints.push_back(Coord(40, 230));
		level.startingPoints.push_back(Coord(510, 35));
		level.startingPoints.push_back(Coord(600, 425));

		level.respawnPoints = level.startingPoints;
		level.respawnPoints.push_back(Coord(50, 45));
		level.respawnPoints.push_back(Coord(590, 135));
		level.respawnPoints.push_back(Coord(600, 320));
	}

	std::vector<shared_ptr<PlayerLogic> > enemies;
	{
		// TODO: Load the StupidAI walking route from file (or implement
		// a better AI...)
		int ar[][2] = {
			{41 , 220}, {64 , 195}, {112, 158}, {151, 133}, {180, 119},
			{191, 115}, {233, 114}, {388, 116}, {406, 121}, {426, 134},
			{433, 155}, {438, 186}, {448, 196}, {462, 201}, {472, 212},
			{478, 221}, {491, 220}, {496, 215}, {501, 203}, {504, 153},
			{502, 102}, {506, 91 }, {512, 89 }, {519, 98 }, {523, 109},
			{524, 218}, {516, 308}, {511, 325}, {504, 339}, {496, 353},
			{488, 407}, {479, 431}, {470, 439}, {456, 439}, {186, 438},
			{163, 435}, {162, 420}, {165, 409}, {177, 406}, {195, 399},
			{203, 394}, {207, 386}, {208, 375}, {207, 359}, {205, 331},
			{200, 320}, {184, 312}, {148, 306}, {95 , 296}, {62 , 289},
			{50 , 279}, {40 , 267}, {37 , 249}, {45 , 229}, {67 , 204},
			{97 , 182}, {112, 172}, {120, 160}, {122, 144}, {119, 110},
			{111, 78 }, {103, 68 }, {90 , 61 }, {67 , 58 }, {36 , 58 },
			{27 , 55 }, {20 , 42 }, {22 , 29 }, {37 , 22 }, {70 , 22 },
			{96 , 31 }, {114, 51 }, {122, 83 }, {132, 104}, {147, 121},
			{162, 122}, {186, 125}, {216, 124}, {252, 124}, {256, 122},
			{263, 115}, {267, 97 }, {264, 45 }, {267, 38 }, {273, 33 },
			{395, 27 }, {420, 40 }, {439, 65 }, {444, 98 }, {446, 131},
			{446, 173}, {445, 200}, {448, 224}, {459, 245}, {471, 272},
			{475, 303}, {480, 354}, {484, 409}, {484, 430}, {480, 439},
			{467, 451}, {444, 456}, {225, 445}, {214, 433}, {204, 409},
			{208, 378}, {207, 314}, {205, 294}, {201, 252}, {199, 205},
			{193, 190}, {185, 174}, {169, 161}, {113, 159}, {86 , 162},
			{60 , 171}, {43 , 182}, {26 , 205}, {21 , 232}, {26 , 256},
			{31 , 276}, {54 , 288}, {91 , 300}, {169, 318}, {208, 330},
			{224, 334}, {404, 332}, {423, 329}, {439, 323}, {443, 311},
			{444, 293}, {441, 179}, {440, 162}, {431, 149}, {397, 139},
			{270, 134}, {249, 138}, {232, 149}, {215, 173}, {214, 228},
			{208, 301}, {216, 341}, {239, 344}, {466, 339}, {485, 357},
			{490, 386}, {484, 411}, {461, 422}, {415, 422}, {250, 414},
			{225, 400}, {215, 381}, {203, 359}, {176, 326}, {124, 304},
			{67 , 285}, {47 , 273}, {35 , 251}, {34 , 241},
		};
		size_t sz = (sizeof ar / sizeof *ar);
		std::vector<Coord> coords;
		for (size_t i = 0; i < sz; ++i) {
			coords.push_back(Coord(ar[i][0], ar[i][1]));
		}
		shared_ptr<PlayerLogic> enemy(new StupidAIPlayer(coords));
		enemies.push_back(enemy);
	}
	return new GameFrame(level, enemies);
}
