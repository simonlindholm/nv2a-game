#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdexcept>
#include <string>
#include "ResourceCache.h"
#include "exceptions.h"
#include "graphicsutil.h"

ResourceCache::ResourceCache() {
	// Zero-initialize surface pointers
	for (int i = 0; i < RotatedImages::End; ++i) {
		for (int j = 0; j < 360; ++j) {
			rotateCache[i].images[j] = 0;
		}
	}
	for (int i = 0; i < StaticImages::End; ++i) {
		cache[i] = 0;
	}

	try {
		// Load rotatable images
		addRotatable("human.png", RotatedImages::Human);
		addRotatable("enemy.png", RotatedImages::Enemy);
		addRotatable("bullet.png", RotatedImages::Bullet);

		// Load static images
		addStatic("player_HP.png", StaticImages::PlayerHP);
		addStatic("enemy_HP.png", StaticImages::EnemyHP);
		addStatic("item-mushroom.png", StaticImages::Mushroom);
		addStatic("item-shield.png", StaticImages::Shield);
		addStatic("item-speed.png", StaticImages::Speed);
		addStatic("icon-shield.png", StaticImages::IconShield);
		addStatic("icon-speed.png", StaticImages::IconSpeed);
		addStatic("aura-shield.png", StaticImages::AuraShield);
		addStatic("map.png", StaticImages::LevelMap);
	}
	catch (...) {
		// Clean up in case of errors
		this->~ResourceCache();
		throw;
	}
}

ResourceCache::~ResourceCache() {
	for (int i = 0; i < RotatedImages::End; ++i) {
		for (int j = 0; j < 360; ++j) {
			SDL_FreeSurface(this->rotateCache[i].images[j]);
		}
	}
	for (int i = 0; i < StaticImages::End; ++i) {
		SDL_FreeSurface(this->cache[i]);
	}
}

static ResourceCache* singleton = 0;
void ResourceCache::init() {
	if (!singleton) singleton = new ResourceCache;
}
void ResourceCache::destroy() {
	delete singleton;
	singleton = 0;
}
ResourceCache& ResourceCache::get() {
	return *singleton;
}

SDL_Surface* ResourceCache::getRotatedImg(RotatedImages::Id it, int angle) const {
	int ind = static_cast<int>(it);
	return rotateCache[ind].images[angle];
}

SDL_Surface* ResourceCache::getImg(StaticImages::Id it) const {
	int ind = static_cast<int>(it);
	return cache[ind];
}

void ResourceCache::addRotatable(const std::string& filename, RotatedImages::Id it) {
	int ind = static_cast<int>(it);
	SDL_Surface* image = loadSurface(filename.c_str());
	for (int i = 0; i < 360; i++){
		SDL_Surface* surf = rotozoomSurface(image, i, 0.0625, 1);
		if (!surf) throw std::runtime_error("Rotozoom didn't work.");
		rotateCache[ind].images[i] = surf;
	}
}
void ResourceCache::addStatic(const std::string& filename, StaticImages::Id it){
	int ind = static_cast<int>(it);
	cache[ind] = loadSurface(filename.c_str());
}
