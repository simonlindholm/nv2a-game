#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdexcept>
#include <string>
#include "GraphicsCache.h"
#include "exceptions.h"
#include "graphicsutil.h"

GraphicsCache::GraphicsCache() {
	//Load rotateable images
	addRotateable("human.png", RotatedImgHuman);
	addRotateable("enemy.png", RotatedImgEnemy);

	//Load static images
	addStatic("bullet.png", ImgBullet);
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

void GraphicsCache::addRotateable(std::string filename, RotatedImageType it){
    SDL_Surface* image = loadSurface(filename.c_str());
	for (int i = 0; i < 360; i++){
		SDL_Surface* surf = rotozoomSurface(image, i, 0.0625, 1);
		if (!surf) throw std::runtime_error("Rotozoom didn't work.");
		rotateCache[it].images[i] = surf;
	}
}
void GraphicsCache::addStatic(std::string filename, ImageType it){
    SDL_Surface* image = loadSurface(filename.c_str());
    cache[it] = image;
}
