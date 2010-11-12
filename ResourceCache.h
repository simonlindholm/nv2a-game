#pragma once
#include <SDL/SDL.h>
#include <string>
#include "Resources.h"
#include "graphicsutil.h"

// ResourceCache is a singleton class used to pre-render every image and rotation
// thereof at startup time. This is done so that the program won't have to rotate
// or load images at runtime, which would be a major performance set-back.
class ResourceCache {
	public:

		SDL_Surface* getImg(StaticImages::Id it) const;
		SDL_Surface* getRotatedImg(RotatedImages::Id it, int angle) const;

		static void init();
		static void destroy();
		static ResourceCache& get();

	private:
		RotatableObject rotateCache[RotatedImages::End];
		SDL_Surface* cache[StaticImages::End];
		void addRotatable(const std::string& filename, RotatedImages::Id it);
		void addStatic(const std::string& filename, StaticImages::Id it);
		// Let constructors/destructors be private, since ResourceCache is a singleton
		ResourceCache();
		~ResourceCache();
		ResourceCache(const ResourceCache&); //Left undefined
};
