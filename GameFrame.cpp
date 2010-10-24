#include <SDL/SDL.h>
#include "GameFrame.h"
#include "SDL_helpers.h"
#include "HumanPlayer.h"
#include "Bullet.h"
#include "Config.h"
#include "exceptions.h"
#include "shared_ptr.h"

GameFrame::GameFrame(const std::vector<shared_ptr<Player> >& enemies)
{
	Coord startingPos[] = {Coord(30, 30), Coord(200, 200)};
	size_t stSize = sizeof startingPos/sizeof *startingPos;

	player = shared_ptr<HumanPlayer>(new HumanPlayer);
	gameState.players = enemies;
	gameState.players.push_back(player);
	for (size_t i = 0; i < gameState.players.size() && i < stSize; ++i) {
		gameState.players[i]->moveTo(startingPos[i]);
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

		Player::Action ac = p->move(gameState, delay);

		Coord pos = p->getPosition();
		Coord npos = pos;
		npos.x += ac.mx;
		npos.y += ac.my;
		p->moveTo(npos);

		// Collision detection, don't move against other players.
		bool stop = false;
		Hitbox hbox = p->getHitbox();
		if (gameState.wall.collidesWith(hbox)) stop = true;
		if (!stop) {
			for (size_t j = 0; j < gameState.players.size(); ++j) {
				if (i != j && gameState.players[j]->getHitbox().collidesWith(hbox)) {
					stop = true;
					break;
				}
			}
		}

		if (stop) {
			// Movement blocked by other player; revert the move
			p->moveTo(pos);
		}

		// Handle shooting
		if (ac.shooting) {
			shared_ptr<Bullet> b(new Bullet(p->getPosition(), p->getAngle(), i));
			gameState.bullets.push_back(b);
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
				shared_ptr<Player> p = gameState.players[i];
				if (bullet->getOwner() != i &&
						p->getHitbox().collidesWith(bhit)) {
					// TODO: Handle player collision
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


	// Draw the player interface
	player->paint(gameState, screen);
	return 0;
}
