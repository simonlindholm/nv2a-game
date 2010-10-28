#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdexcept>
#include <string>
#include "GraphicsCache.h"
#include "exceptions.h"
#include "graphicsutil.h"

GraphicsCache::GraphicsCache() {
	// Zero-initialize surface pointers
	for (int i = 0; i < RotatedImgEnd; ++i) {
		for (int j = 0; j < 360; ++j) {
			this->rotateCache[i].images[j] = 0;
		}
	}
	for (int i = 0; i < ImgEnd; ++i) {
		this->cache[i] = 0;
	}

	try {
		// Load rotatable images
		addRotatable("human.png", RotatedImgHuman);
		addRotatable("enemy.png", RotatedImgEnemy);

		// Load static images
		addStatic("bullet.png", ImgBullet);
		addStatic("player_HP.png", ImgPlayerHP);
		addStatic("enemy_HP.png", ImgEnemyHP);
	}
	catch (...) {
		// Clean up in case of errors
		this->~GraphicsCache();
		throw;
	}
}

GraphicsCache::~GraphicsCache() {
	for (int i = 0; i < RotatedImgEnd; ++i) {
		for (int j = 0; j < 360; ++j) {
			SDL_FreeSurface(this->rotateCache[i].images[j]);
		}
	}
	for (int i = 0; i < ImgEnd; ++i) {
		SDL_FreeSurface(this->cache[i]);
	}
}

static GraphicsCache* singleton = 0;
void GraphicsCache::init() {
	if (!singleton) singleton = new GraphicsCache;
}
void GraphicsCache::destroy() {
	delete singleton;
	singleton = 0;
}
GraphicsCache& GraphicsCache::get() {
	return *singleton;
}

SDL_Surface* GraphicsCache::getRotatedImg(RotatedImageType it, int angle) const {
	return this->rotateCache[it].images[angle];
}

SDL_Surface* GraphicsCache::getImg(ImageType it) const {
	return this->cache[it];
}

void GraphicsCache::addRotatable(const std::string& filename, RotatedImageType it){
	SDL_Surface* image = loadSurface(filename.c_str());
	for (int i = 0; i < 360; i++){
		SDL_Surface* surf = rotozoomSurface(image, i, 0.0625, 1);
		if (!surf) throw std::runtime_error("Rotozoom didn't work.");
		rotateCache[it].images[i] = surf;
	}
}
void GraphicsCache::addStatic(const std::string& filename, ImageType it){
	SDL_Surface* image = loadSurface(filename.c_str());
	cache[it] = image;
}
