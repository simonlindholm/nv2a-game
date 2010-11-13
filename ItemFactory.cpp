#include "ItemFactory.h"
#include "util.h"
#include "shared_ptr.h"

shared_ptr<Item> ItemFactory::createItem(const Coord& pos) {
	shared_ptr<Item> ret;
	int randomNumber = randRange(0, 3);

	switch(randomNumber) {
		case 0:
			ret = shared_ptr<Item>(new MushroomItem(pos));
			break;
		case 1:
			ret = shared_ptr<Item>(new ShieldItem(pos));
			break;
		case 2:
			ret = shared_ptr<Item>(new SpeedItem(pos));
			break;
	}

	return ret;
}
