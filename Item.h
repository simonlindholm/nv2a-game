#pragma once
#include "Hitbox.h"
#include "GraphicsCache.h"
#include "fwd.h"
#include "util.h"

// Abstract class symbolising an item
class Item {
	private:
		Coord pos;
		Hitbox hitbox;
	public:
		virtual ~Item();
		void init(const Coord& pos, const Hitbox& hitbox);
		Hitbox getHitbox() const;
		Coord getPosition() const;
		virtual void use(PlayerInfo& pl) = 0;
		virtual GraphicsCache::ImageType getImage() const = 0;
};

class MushroomItem : public Item {
	public:
		MushroomItem(const Coord& pos);
		virtual void use(PlayerInfo& pl);
		virtual GraphicsCache::ImageType getImage() const;
};