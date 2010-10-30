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

void MushroomItem::use(Player& pl) {
	int hp = pl.getHP();
	hp += 50;
	if (hp > 100) hp = 100; //TODO: Implement a reduceHP-like function.
	pl.setHP(hp);
}
