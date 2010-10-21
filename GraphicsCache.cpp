#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "GraphicsCache.h"
#include "graphicsutil.h"
#include "SDL/SDL_rotozoom.h"

GraphicsCache::GraphicsCache() {
    SDL_Surface* human = loadSurface("char.png");
    for (int i = 0; i < 360; i++){
        SDL_Surface* rotate = rotozoomSurface(human, i, 0.1, 1);
        cache[ImgHuman].images[i] = rotate;
    }
}

GraphicsCache::~GraphicsCache() {
    for (int a = 0; a < this->ImgEnd; a++) {
        for (int b = 0; b < 360; b++) {
            SDL_FreeSurface(this->cache[a].images[b]);
        }
    }
}

GraphicsCache& GraphicsCache::get() {
    static GraphicsCache singleton;
    return singleton;
}

SDL_Surface* GraphicsCache::getImg(ImageType it, int angle) const {
    return this->cache[it].images[angle];
}
