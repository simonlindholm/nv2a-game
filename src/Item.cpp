#include "Item.h"
#include "Player.h"
#include "Hitbox.h"
#include "Buff.h"
#include "shared_ptr.h"

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

StaticImages::Id MushroomItem::getImage() const {
	return StaticImages::Mushroom;
}


ShieldItem::ShieldItem(const Coord& pos) {
	Hitbox hb;
	hb.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 7.5)));
	this->Item::init(pos, hb);
}

void ShieldItem::use(PlayerInfo& pl) {
	shared_ptr<Buff> shieldBuff(new ShieldBuff(pl));
	pl.addBuff(shieldBuff);
}

StaticImages::Id ShieldItem::getImage() const {
	return StaticImages::Shield;
}


SpeedItem::SpeedItem(const Coord& pos) {
	Hitbox hb;
	hb.add(shared_ptr<Shape>(new Circle(Coord(0, 0), 7.5)));
	this->Item::init(pos, hb);
}

void SpeedItem::use(PlayerInfo &pl) {
	shared_ptr<Buff> speedBuff(new SpeedBuff(pl));
	pl.addBuff(speedBuff);
}

StaticImages::Id SpeedItem::getImage() const {
	return StaticImages::Speed;
}
