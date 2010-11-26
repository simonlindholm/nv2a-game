#include <SDL/SDL.h>
#include <cmath>
#include "GameFrame.h"
#include "SDL_helpers.h"
#include "HumanPlayer.h"
#include "Timer.h"
#include "Bullet.h"
#include "ItemFactory.h"
#include "Config.h"
#include "util.h"
#include "mathutil.h"
#include "exceptions.h"
#include "shared_ptr.h"

GameFrame::GameFrame(const Level& level,
		const std::vector<shared_ptr<Player> >& enemies)
{
	player = shared_ptr<HumanPlayer>(new HumanPlayer);
	gameState.players = enemies;
	gameState.players.push_back(player);

	gameState.level = level;

	gameState.playerInfo.resize(gameState.players.size());

	// TODO: Start at the points given by level.startPoints instead. (The
	// AI doesn't handle random starting positions very well yet, though.)
	Coord startingPos[] = {Coord(40, 230), Coord(600, 425)};
	size_t stSize = sizeof startingPos/sizeof *startingPos;
	for (size_t i = 0; i < gameState.players.size() && i < stSize; ++i) {
		gameState.playerInfo[i].moveTo(startingPos[i]);
	}

	for (size_t i = 0; i < gameState.players.size(); ++i) {
		gameState.players[i]->setInfo(&gameState.playerInfo[i]);
	}

	// Add out-of-screen rectangles to act as boundaries
	const int wallSize = 500;
	int sw = Config::get().winWidth, sh = Config::get().winHeight;

	gameState.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, -wallSize),
					sw + 2*wallSize, wallSize)));
	gameState.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, -wallSize),
					wallSize, sw + 2*wallSize)));
	gameState.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, sh),
					sw + 2*wallSize, wallSize)));
	gameState.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(sw, -wallSize),
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
		ItemFactory itemFactory;
		shared_ptr<Item> ret = itemFactory.createItem(pos);
		if (gameState.level.wall.collidesWith(ret->getHitbox())) continue;
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
		Coord acm;
		acm.x = ac.mx;
		acm.y = ac.my;

		Coord pos = pinfo.getPosition(), nvec;

		for (int it = 0; it < 2; ++it) {
			Coord npos = pos;
			npos.x += acm.x;
			npos.y += acm.y;
			pinfo.moveTo(npos);

			// Collision detection, don't move against other players or the wall
			Hitbox hbox = pinfo.getHitbox();
			int stop = hbox.collidesWithSurf(gameState.level.wall, acm, nvec);
			for (size_t j = 0; j < gameState.players.size(); ++j) {
				if (i == j) continue;
				if (stop) break;
				stop = hbox.collidesWithSurf(gameState.playerInfo[j].getHitbox(), acm, nvec);
			}

			if (stop) {
				// Movement blocked; revert the move
				pinfo.moveTo(pos);
			}

			if (stop == 2 && it == 0) {
				// Reduce movement against surface to 0, and apply friction
				nvec.y = -nvec.y;
				acm = rotatePointVec(acm, nvec);
				double fr = std::abs(acm.x) * 0.15;
				if (acm.y > 0) {
					acm.y -= fr;
					if (acm.y < 0) acm.y = 0;
				}
				else {
					acm.y += fr;
					if (acm.y > 0) acm.y = 0;
				}
				acm.x = 0;
				nvec.y = -nvec.y;
				acm = rotatePointVec(acm, nvec);
			}
			else break;
		}

		// Handle shooting
		if (ac.shooting) {
			if(pinfo.canShoot()) {
				shared_ptr<Bullet> b(new Bullet(pinfo.getPosition(), pinfo.getAngle(), i));
				gameState.bullets.push_back(b);
				pinfo.restartShotTimer();
			}
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

		if (bhit.collidesWith(gameState.level.wall)) {
			//TODO: Handle wall collision
			del = true;
		}
		else {
			for (size_t i = 0; i < gameState.players.size(); ++i) {
				PlayerInfo& p = gameState.playerInfo[i];
				if (bullet->getOwner() != i &&
						p.getHitbox().collidesWith(bhit)) {
					p.setHP(p.getHP() - (int)(bullet->getDamage() - p.getDef()));
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

	for (size_t i = 0; i < gameState.players.size(); ++i) {
		PlayerInfo& p = gameState.playerInfo[i];

		// Step the timers in PlayerInfo
		p.step(delay);
	}

	// Draw the player interface
	player->paint(gameState, screen);
	return 0;
}
