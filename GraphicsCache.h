#pragma once
#include <SDL/SDL.h>
#include <string>
#include "graphicsutil.h"

// GraphicsCache is a singleton class used to pre-render every image and rotation
// thereof at startup time. This is done so that the program won't have to rotate
// or load images at runtime, which would be a major performance set-back.
class GraphicsCache {
	public:
		enum RotatedImageType {
			RotatedImgHuman,
			RotatedImgEnemy,
			RotatedImgEnd
		};

		enum ImageType {
			ImgBullet,
			ImgEnd
		};

		static GraphicsCache& get();
		SDL_Surface* getImg(ImageType it) const;
		SDL_Surface* getRotatedImg(RotatedImageType it, int angle) const;

	private:
		RotatableObject rotateCache[RotatedImgEnd];
		SDL_Surface* cache[ImgEnd];
        void addRotateable(std::string filename, RotatedImageType it);
        void addStatic(std::string filename, ImageType it);
		// Let constructors/destructors be private, since GraphicsCache is a singleton
		GraphicsCache();
		~GraphicsCache();
		GraphicsCache(const GraphicsCache&); //Left undefined
};
