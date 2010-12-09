#include <SDL/SDL.h>
#include <cmath>
#include "HumanPlayer.h"
#include "GameState.h"
#include "Bullet.h"
#include "Item.h"
#include "SDL_helpers.h"
#include "util.h"
#include "mathutil.h"
#include "graphicsutil.h"
#include "ResourceCache.h"
#include "Config.h"

HumanPlayer::HumanPlayer() {
}

void HumanPlayer::startFrame(Uint8* keyState, Uint8 mouseState, int mouseX, int mouseY) {
	// Calculate relative movement direction (ie. left, up, diagonal, etc.)
	vx = vy = 0;
	if (keyState[SDLK_w]) ++vy;
	if (keyState[SDLK_a]) --vx;
	if (keyState[SDLK_s]) --vy;
	if (keyState[SDLK_d]) ++vx;

	mouse.x = mouseX;
	mouse.y = mouseY;

	shooting = false;
	if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) shooting = true;
}

void HumanPlayer::handleEvent(const SDL_Event& event) {
	// TODO: Handle events related to the player interface, such as
	// weapon swapping, item use, etc.
}

void HumanPlayer::paint(const GameState& game, SDL_Surface* screen) {
	// TODO: Paint player interface to screen
	// (The GUI that includes score, weapons, etc. goes here)

	SDL_Lock lock(screen);
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 100, 0, 255));
	ResourceCache& rCache = ResourceCache::get();
	Config& cfg = Config::get();

	// Draw the background
	SDL_Rect bgPos;
	bgPos.x = (cfg.winWidth - game.level.bg->w) / 2;
	bgPos.y = (cfg.winHeight - game.level.bg->h) / 2;
	SDL_BlitSurface(game.level.bg, 0, screen, &bgPos);

	// Draw the items
	std::list<shared_ptr<Item> >::const_iterator iIter, iEnd;
	iIter = game.items.begin();
	iEnd = game.items.end();
	while (iIter != iEnd) {
		const Item& item = **iIter;
		StaticImages::Id img = item.getImage();
		Coord pos = item.getPosition();

		drawCenteredSurface(pos, rCache.getImg(img), screen);

		++iIter;
	}

	// Then the bullets
	typedef std::list<shared_ptr<Bullet> >::const_iterator BulletIt;
	BulletIt bIter = game.bullets.begin(), bEnd = game.bullets.end();
	while (bIter != bEnd) {
		shared_ptr<Bullet> b = *bIter;
		Coord pos = b->getPosition();

		drawCenteredSurface(pos, rCache.getImg(StaticImages::Bullet), screen);

		++bIter;
	}

	// And lastly the players
	for (size_t i = 0; i < game.players.size(); ++i) {
		const Player& pl = *game.players[i];
		const PlayerInfo& p = game.playerInfo[i];
		Coord pos = p.getPosition();
		int angle = radToIntDeg(p.getAngle());

		if (&pl == this) {
			// Let yourself have a separate appearance
			drawCenteredSurface(pos, rCache.getRotatedImg(RotatedImages::Human, angle), screen);

			// Show player's HP bar in the lower left corner
			SDL_Rect clipHP = {(Sint16)(1.5*(100 - p.getHP())), 0, 150, 15};
			SDL_Rect HPpos = {(Sint16)(screen->h / 30), (Sint16)(screen->h - (screen->h / 15)), 0, 0};
			SDL_BlitSurface(rCache.getImg(StaticImages::PlayerHP), &clipHP, screen, &HPpos);
		}
		else {
			drawCenteredSurface(pos, rCache.getRotatedImg(RotatedImages::Enemy, angle), screen);

			SDL_Rect clipHP = {(Sint16)(0.6*(100 - p.getHP())), 0, 60, 8};
			SDL_Rect HPpos = {(Sint16)(pos.x - 30), (Sint16)(pos.y - 30), 0, 0};
			SDL_BlitSurface(rCache.getImg(StaticImages::EnemyHP), &clipHP, screen, &HPpos);
		}
	}
}

// Calculate movement of player
Player::Action HumanPlayer::move(const GameState& game, unsigned int delay) {
	bool atCursor = false;
	double angle = info->getAngle();

	// Find forward angle
	Coord pos = info->getPosition();
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
		angle = std::atan2(-relY, relX);
	}

	// Calculate movement
	double mx, my;
	double mov = delay * info->getSpeed();
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

	Player::Action a;
	a.mx = mx;
	a.my = my;
	a.shooting = this->shooting;
	a.angle = angle;
	return a;
}
