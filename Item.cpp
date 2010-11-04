#include "Item.h"
#include "Player.h"
#include "Hitbox.h"

Item::~Item() {}

void Item::init(const Coord& pos, const Hitbox& hitbox) {
	this->pos = pos;
	this->hitbox = hitbox;
	this->hitbox.moveBy(pos);
}

Coord Item::getPosition() const {
	return this->pos;
}

Hitbox Item::getHitbox() const {
	return this->hitbox;
}


MushroomItem::MushroomItem(const Coord& pos) {
	Hitbox hb;
	hb.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 7.5)));
	this->Item::init(pos, hb);
}

void MushroomItem::use(PlayerInfo& pl) {
	pl.setHP(pl.getHP() + 50);
}

GraphicsCache::ImageType MushroomItem::getImage() const {
	return GraphicsCache::ImgMushroom;
}


ShieldItem::ShieldItem(const Coord& pos) {
	Hitbox hb;
	hb.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 7.5)));
	this->Item::init(pos, hb);
}

void ShieldItem::use(PlayerInfo& pl) {
	pl.setDef(10);
	pl.setBuffTimer(5000);
}

GraphicsCache::ImageType ShieldItem::getImage() const {
	return GraphicsCache::ImgShield;
}