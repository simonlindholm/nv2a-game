#include "ItemFactory.h"
#include "util.h"
#include "shared_ptr.h"

shared_ptr<Item> ItemFactory::createItem(const Coord& pos) {
	Item* ret = NULL;
	int randomNumber = randTo(3);

	switch(randomNumber) {
		case 0:
			ret = new MushroomItem(pos);
			break;
		case 1:
			ret = new ShieldItem(pos);
			break;
		case 2:
			ret = new SpeedItem(pos);
			break;
	}

	return shared_ptr<Item>(ret);
}
