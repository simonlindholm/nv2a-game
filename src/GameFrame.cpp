#include <SDL/SDL.h>
#include <algorithm>
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
		const std::vector<shared_ptr<PlayerLogic> >& enemies)
{
	player = shared_ptr<HumanPlayer>(new HumanPlayer);
	for (size_t i = 0; i < enemies.size(); ++i) {
		shared_ptr<Player> pl(new Player(enemies[i]));
		gs.players.push_back(pl);
	}
	gs.players.push_back(shared_ptr<Player>(new Player(player)));

	gs.level = level;

	// Start the players at random positions, avoiding collisions by starting
	// them at a rearrangement of the possible positions instead of choosing
	// each position independently.
	std::vector<Coord> stPoints = level.startingPoints;
	std::random_shuffle(stPoints.begin(), stPoints.end());
	for (size_t i = 0; i < gs.players.size(); ++i) {
		gs.players[i]->info.moveTo(stPoints[i % stPoints.size()]);
		gs.players[i]->logic->signalSpawn();
	}

	// Add out-of-screen rectangles to act as boundaries
	const int wallSize = 500;
	int sw = Config::get().winWidth, sh = Config::get().winHeight;

	gs.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, -wallSize),
					sw + 2*wallSize, wallSize)));
	gs.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, -wallSize),
					wallSize, sw + 2*wallSize)));
	gs.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(-wallSize, sh),
					sw + 2*wallSize, wallSize)));
	gs.level.wall.add(shared_ptr<Shape>(new Rectangle(Coord(sw, -wallSize),
					wallSize, sw + 2*wallSize)));

	gs.itemsLeft = 0;
	this->setItemTimer();
}

void GameFrame::setItemTimer() {
	const int maxItems = 2, minTime = 7000, maxTime = 10000;
	if (gs.itemsLeft < maxItems && !gs.itemTimer.isActive()) {
		gs.itemTimer.set(randRange(minTime, maxTime));
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
		if (gs.level.wall.collidesWith(ret->getHitbox())) continue;
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
	BulletIt bIter = gs.bullets.begin(), bEnd = gs.bullets.end();
	while (bIter != bEnd) {
		shared_ptr<Bullet> b = *bIter;
		b->move(delay);
		++bIter;
	}

	// Move all players
	for (size_t i = 0; i < gs.players.size(); ++i) {
		Player& p = *gs.players[i];
		PlayerInfo& pinfo = p.info;

		PlayerLogic::Action ac = p.logic->move(gs, delay);
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
			int stop = hbox.collidesWithSurf(gs.level.wall, acm, nvec);
			for (size_t j = 0; j < gs.players.size(); ++j) {
				if (i == j) continue;
				if (stop) break;
				stop = hbox.collidesWithSurf(gs.players[j]->info.getHitbox(), acm, nvec);
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
				gs.bullets.push_back(b);
				pinfo.restartShotTimer();
			}
		}

		// Handle rotation
		pinfo.setAngle(ac.angle);
	}

	// Appearance of new items
	gs.itemTimer.step(delay);
	if (gs.itemTimer.isDone()) {
		shared_ptr<Item> newItem = this->getRandomItem();
		gs.items.push_back(newItem);
		++gs.itemsLeft;
		gs.itemTimer.stop();
		this->setItemTimer();
	}

	// Check item collisions
	std::list<shared_ptr<Item> >::iterator iIter, iEnd;
	iIter = gs.items.begin();
	iEnd = gs.items.end();
	while (iIter != iEnd) {
		shared_ptr<Item> item = *iIter;
		const Hitbox& ihit = item->getHitbox();
		bool del = false;
		for (size_t i = 0; i < gs.players.size(); ++i) {
			PlayerInfo& p = gs.players[i]->info;
			if (p.getHitbox().collidesWith(ihit)) {
				item->use(p);

				del = true;
				break;
			}
		}

		if (del) {
			gs.items.erase(iIter++);
			--gs.itemsLeft;
			this->setItemTimer();
		}
		else {
			++iIter;
		}
	}

	// Check collisions for bullets
	bIter = gs.bullets.begin();
	bEnd = gs.bullets.end();
	while (bIter != bEnd) {
		shared_ptr<Bullet> bullet = *bIter;
		const Hitbox& bhit = bullet->getHitbox();
		bool del = false;

		if (bhit.collidesWith(gs.level.wall)) {
			//TODO: Handle wall collision
			del = true;
		}
		else {
			for (size_t i = 0; i < gs.players.size(); ++i) {
				Player& pl = *gs.players[i];
				PlayerInfo& p = pl.info;
				if (bullet->getOwner() != i &&
						p.getHitbox().collidesWith(bhit)) {
					p.setHP(p.getHP() - (int)(bullet->getDamage() - p.getDef()));
					p.resetRegenTimer();

					if(p.getHP() <= 0) {
						// Player died, respawn at a random position
						// TODO: Change this to something better.
						// TODO: Reset buffs, timers, etc. This should probably
						// be in a PlayerInfo member function, together with
						// the HP reset.
						std::vector<Coord>& rpoints = gs.level.respawnPoints;
						p.moveTo(rpoints[randTo(rpoints.size())]);
						p.setHP(100);
						pl.logic->signalSpawn();
					}

					del = true;
					break;
				}
			}
		}

		if (del) {
			gs.bullets.erase(bIter++);
		}
		else {
			++bIter;
		}
	}

	for (size_t i = 0; i < gs.players.size(); ++i) {
		PlayerInfo& p = gs.players[i]->info;

		// Step the timers in PlayerInfo
		p.step(delay);
	}

	// Draw the player interface
	player->paint(gs, screen);
	return 0;
}
