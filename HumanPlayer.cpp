#include <SDL/SDL.h>
#include <cmath>
#include "HumanPlayer.h"
#include "GameState.h"
#include "Bullet.h"
#include "SDL_helpers.h"
#include "util.h"
#include "mathutil.h"
#include "graphicsutil.h"
#include "GraphicsCache.h"

HumanPlayer::HumanPlayer() {
}

void HumanPlayer::startFrame(Uint8* keyState, Uint8 mouseState, int mouseX, int mouseY) {
	// Calculate relative movement direction (ie. left, up, diagonal, etc.)
	vx = vy = 0;
	if (keyState[SDLK_w]) ++vy;
	if (keyState[SDLK_a]) --vx;
	if (keyState[SDLK_s]) --vy;
	if (keyState[SDLK_d]) ++vx;

	this->mouse.x = mouseX;
	this->mouse.y = mouseY;

	this->shooting = false;
}

void HumanPlayer::handleEvent(const SDL_Event& event) {
	// TODO: Handle events related to the player interface, such as
	// weapon swapping, item use, etc.
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		this->shooting = true;
	}
}

void HumanPlayer::paint(const GameState& game, SDL_Surface* screen) {
	// TODO: Paint player interface to screen
	// (The GUI that includes score, weapons, etc. goes here)

	SDL_Lock lock(screen);
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 100, 0, 255));
	GraphicsCache& gCache = GraphicsCache::get();

	for (size_t i = 0; i < game.players.size(); ++i) {
		const Player& p = *game.players[i];
		Coord pos = p.getPosition();
		int angle = radToIntDeg(p.getAngle());


		if (&p == this) {
			// Let yourself have a seperate appearance
			drawCenteredSurface(pos, gCache.getRotatedImg(GraphicsCache::RotatedImgHuman, angle), screen);

			// Show player's HP bar in the lower left corner
			SDL_Rect clipHP = {(int)(1.5*(100 - p.getHP())), 0, 150, 15};
			SDL_Rect HPpos = {(screen->h / 30), screen->h - (screen->h / 15), 0, 0};
			SDL_BlitSurface( gCache.getImg(GraphicsCache::ImgPlayerHP), &clipHP, screen, &HPpos );
		}
		else {
			drawCenteredSurface(pos, gCache.getRotatedImg(GraphicsCache::RotatedImgEnemy, angle), screen);

			SDL_Rect clipHP = {(int)(0.6*(100 - p.getHP())), 0, 60, 8};
			SDL_Rect HPpos = {(int)pos.x - 30, (int)pos.y - 30, 0, 0};
			SDL_BlitSurface( gCache.getImg(GraphicsCache::ImgEnemyHP), &clipHP, screen, &HPpos );
		}
	}

	typedef std::list<shared_ptr<Bullet> >::const_iterator BulletIt;
	BulletIt bIter = game.bullets.begin(), bEnd = game.bullets.end();
	while (bIter != bEnd) {
		shared_ptr<Bullet> b = *bIter;
		Coord pos = b->getPosition();

		drawCenteredSurface(pos, gCache.getImg(GraphicsCache::ImgBullet), screen);

		++bIter;
	}
}

// Calculate movement of player
Player::Action HumanPlayer::move(const GameState& game, unsigned int delay) {
	bool atCursor = false;

	// Find forward angle
	Coord pos = this->getPosition();
	double relX = mouse.x - pos.x;
	double relY = mouse.y - pos.y;
	if (fpEqual(pos.x, mouse.x, 2) && fpEqual(pos.y, mouse.y, 2)) {
		// The player is at the cursor; only move backwards, and retain the
		// old angle of movement (since we can't calculate a new one, and it
		// is nice to be able to back out again the same way we came.)
		atCursor = true;
	}
	else {
		// The vector (mouse - pos) in standard, mathy form is (relX, -relY)
		double angle = std::atan2(-relY, relX);
		angle = reduceAngle(angle);

		this->setAngle(angle);
	}

	// Calculate movement
	double mx, my;
	double angle = this->getAngle();
	double mov = delay * this->getSpeed();
	double cursorDist = pyth(relX, relY);
	double mForward = mov * ((vx && vy) ? std::sqrt(0.5) : 1.0);

	if (vy > 0 && mForward >= cursorDist) {
		// We will reach the cursor in this step, so teleport it there.
		mx = relX;
		my = relY;
	}
	else if ((vy >= 0 && atCursor) || (!vx && !vy)) {
		// Player is either at cursor and not moving backwards, or
		// not moving at all; don't move
		mx = my = 0;
	}
	else if (vx || vy) {
		// Move the player normally

		if (vx && !vy) {
			// Player is moving sidewards, and this is equivalent to
			// moving in a circle around the cursor.
			double mAngle = mov / cursorDist;
			if (vx < 0) mAngle = -mAngle;

			mAngle += angle;

			double nx = mouse.x - cursorDist * std::cos(mAngle);
			double ny = mouse.y + cursorDist * std::sin(mAngle);
			mx = nx - pos.x;
			my = ny - pos.y;
		}
		else {
			// Calculate the movement by walking in the average of the
			// direction angles. This is not entirely accurate, but works
			// well enough.
			double mAngle = 0;
			if (vx < 0) mAngle =  m_pi/2;
			if (vx > 0) mAngle = -m_pi/2;
			if (vy) {
				mAngle /= 2;
				if (vy < 0) {
					mAngle = m_pi - mAngle;
				}
			}

			mAngle += angle;

			// Transform back into regular form
			mx = mov * std::cos(mAngle);
			my = mov * -std::sin(mAngle);
		}
	}

	Action a;
	a.mx = mx;
	a.my = my;
	a.shooting = this->shooting;
	return a;
}
