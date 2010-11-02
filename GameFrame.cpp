#include <SDL/SDL.h>
#include "GameFrame.h"
#include "SDL_helpers.h"
#include "HumanPlayer.h"
#include "Timer.h"
#include "Bullet.h"
#include "Item.h"
#include "Config.h"
#include "util.h"
#include "exceptions.h"
#include "shared_ptr.h"

GameFrame::GameFrame(const std::vector<shared_ptr<Player> >& enemies)
{
	player = shared_ptr<HumanPlayer>(new HumanPlayer);
	gameState.players = enemies;
	gameState.players.push_back(player);

	gameState.playerInfo.resize(gameState.players.size());
	Coord startingPos[] = {Coord(30, 30), Coord(200, 200)};
	size_t stSize = sizeof startingPos/sizeof *startingPos;
	for (size_t i = 0; i < gameState.players.size() && i < stSize; ++i) {
		gameState.playerInfo[i].moveTo(startingPos[i]);
	}

	for (size_t i = 0; i < gameState.players.size(); ++i) {
		gameState.players[i]->setInfo(&gameState.playerInfo[i]);
	}

	// Add out-of-screen rectangles to act as boundaries
	const int wallSize = 100;
	int sw = Config::get().winWidth, sh = Config::get().winHeight;

	gameState.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, -wallSize),
					sw + 2*wallSize, wallSize)));
	gameState.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, -wallSize),
					wallSize, sw + 2*wallSize)));
	gameState.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, sh),
					sw + 2*wallSize, wallSize)));
	gameState.wall.add(shared_ptr<Shape>(new Rectangle(Coord(sw, -wallSize),
					wallSize, sw + 2*wallSize)));

	gameState.itemsLeft = 0;
	this->setItemTimer();
}

void GameFrame::setItemTimer() {
	const int maxItems = 2, minTime = 7000, maxTime = 10000;
	if (gameState.itemsLeft < maxItems && !gameState.itemTimer.isActive()) {
		gameState.itemTimer.set(randRange(minTime, maxTime));
	}
}

shared_ptr<Item> GameFrame::getRandomItem() const {
	Config& cfg = Config::get();
	for (;;) {
		Coord pos;
		pos.x = randRange(0, cfg.winWidth);
		pos.y = randRange(0, cfg.winHeight);
		shared_ptr<Item> ret = shared_ptr<Item>(new MushroomItem(pos));
		if (gameState.wall.collidesWith(ret->getHitbox())) continue;
		return ret;
	}
}

Frame* GameFrame::frame(SDL_Surface* screen, unsigned int delay) {
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

	// Move bullets
	typedef std::list<shared_ptr<Bullet> >::iterator BulletIt;
	BulletIt bIter = gameState.bullets.begin(), bEnd = gameState.bullets.end();
	while (bIter != bEnd) {
		shared_ptr<Bullet> b = *bIter;
		b->move(delay);
		++bIter;
	}

	// Move all players
	for (size_t i = 0; i < gameState.players.size(); ++i) {
		shared_ptr<Player> p = gameState.players[i];
		PlayerInfo& pinfo = gameState.playerInfo[i];

		Player::Action ac = p->move(gameState, delay);

		Coord pos = pinfo.getPosition();
		Coord npos = pos;
		npos.x += ac.mx;
		npos.y += ac.my;
		pinfo.moveTo(npos);

		// Collision detection, don't move against other players.
		bool stop = false;
		Hitbox hbox = pinfo.getHitbox();
		if (gameState.wall.collidesWith(hbox)) stop = true;
		if (!stop) {
			for (size_t j = 0; j < gameState.players.size(); ++j) {
				if (i == j) continue;
				if (gameState.playerInfo[j].getHitbox().collidesWith(hbox)) {
					stop = true;
					break;
				}
			}
		}

		if (stop) {
			// Movement blocked by other player; revert the move
			pinfo.moveTo(pos);
		}

		// Handle shooting
		if (ac.shooting) {
			shared_ptr<Bullet> b(new Bullet(pinfo.getPosition(), pinfo.getAngle(), i));
			gameState.bullets.push_back(b);
		}

		// Handle rotation
		pinfo.setAngle(ac.angle);
	}

	// Appearance of new items
	gameState.itemTimer.step(delay);
	if (gameState.itemTimer.isDone()) {
		shared_ptr<Item> newItem = this->getRandomItem();
		gameState.items.push_back(newItem);
		++gameState.itemsLeft;
		gameState.itemTimer.stop();
		this->setItemTimer();
	}

	// Check item collisions
	std::list<shared_ptr<Item> >::iterator iIter, iEnd;
	iIter = gameState.items.begin();
	iEnd = gameState.items.end();
	while (iIter != iEnd) {
		shared_ptr<Item> item = *iIter;
		const Hitbox& ihit = item->getHitbox();
		bool del = false;
		for (size_t i = 0; i < gameState.players.size(); ++i) {
			PlayerInfo& p = gameState.playerInfo[i];
			if (p.getHitbox().collidesWith(ihit)) {
				item->use(p);
				del = true;
				break;
			}
		}

		if (del) {
			gameState.items.erase(iIter++);
			--gameState.itemsLeft;
			this->setItemTimer();
		}
		else {
			++iIter;
		}
	}

	// Check collisions for bullets
	bIter = gameState.bullets.begin();
	bEnd = gameState.bullets.end();
	while (bIter != bEnd) {
		shared_ptr<Bullet> bullet = *bIter;
		const Hitbox& bhit = bullet->getHitbox();
		bool del = false;

		if (bhit.collidesWith(gameState.wall)) {
			//TODO: Handle wall collision
			del = true;
		}
		else {
			for (size_t i = 0; i < gameState.players.size(); ++i) {
				PlayerInfo& p = gameState.playerInfo[i];
				if (bullet->getOwner() != i &&
						p.getHitbox().collidesWith(bhit)) {
					p.setHP(p.getHP() - (int)bullet->getDamage());
					p.resetRegenTimer();

					//TODO: Game over-screen when HP reaches zero instead of resetting HP
					if(p.getHP() <= 0) {
						p.setHP(100);
					}

					del = true;
					break;
				}
			}
		}

		if (del) {
			gameState.bullets.erase(bIter++);
		}
		else {
			++bIter;
		}
	}

	//Handle HP regeneration
	for (size_t i = 0; i < gameState.players.size(); ++i) {
		if(gameState.playerInfo[i].getHP() < 100) {
			gameState.playerInfo[i].regenTimerLogic(delay);
		}
	}

	// Draw the player interface
	player->paint(gameState, screen);
	return 0;
}
