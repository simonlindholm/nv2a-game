#include <SDL/SDL.h>
#include <cmath>
#include "HumanPlayer.h"
#include "GameState.h"
#include "SDL_helpers.h"
#include "util.h"
#include "mathutil.h"

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

	for(unsigned int i = 0; i < game.players.size(); ++i) {
		const Player& p = *game.players[i];
		Coord pos = p.getPosition();
		Uint32 color;
		if (&p == this) {
			// Let yourself have another color
			color = SDL_MapRGB(screen->format, 0, 255, 0);
		}
		else {
			color = SDL_MapRGB(screen->format, 255, 0, 0);
		}
		SDL_Rect rect;
		rect.x = pos.x - 10;
		rect.y = pos.y - 10;
		rect.w = rect.h = 20;
		SDL_FillRect(screen, &rect, color);
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
	double cursorDist = std::sqrt(relY*relY + relX*relX);
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
	a.shooting = false;
	a.shootingAngle = angle;
	return a;
}
