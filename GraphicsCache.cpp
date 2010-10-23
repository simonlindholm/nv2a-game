#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdexcept>
#include "GraphicsCache.h"
#include "exceptions.h"
#include "graphicsutil.h"

GraphicsCache::GraphicsCache() {
	//Load rotateable images
	SDL_Surface* human = loadSurface("human.png");
	for (int i = 0; i < 360; i++){
		SDL_Surface* surf = rotozoomSurface(human, i, 0.0625, 1);
		if (!surf) throw std::runtime_error("Rotozoom didn't work.");
		rotateCache[RotatedImgHuman].images[i] = surf;
	}
	SDL_Surface* enemy = loadSurface("enemy.png");
	for (int i = 0; i < 360; i++){
		SDL_Surface* surf = rotozoomSurface(enemy, i, 0.0625, 1);
		if (!surf) throw std::runtime_error("Rotozoom didn't work.");
		rotateCache[RotatedImgEnemy].images[i] = surf;
	}

	//Load static images
	SDL_Surface* bullet = loadSurface("bullet.png");
	cache[ImgBullet] = bullet;
}

GraphicsCache::~GraphicsCache() {
	for (int a = 0; a < this->ImgEnd; a++) {
		for (int b = 0; b < 360; b++) {
			SDL_FreeSurface(this->rotateCache[a].images[b]);
		}
	}
}

GraphicsCache& GraphicsCache::get() {
	static GraphicsCache singleton;
	return singleton;
}

SDL_Surface* GraphicsCache::getRotatedImg(RotatedImageType it, int angle) const {
	return this->rotateCache[it].images[angle];
}

SDL_Surface* GraphicsCache::getImg(ImageType it) const {
	return this->cache[it];
}
