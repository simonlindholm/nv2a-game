#include "ItemFactory.h"
#include "util.h"

Item* ItemFactory::createItem(const Coord& pos) {
	int randomNumber = randRange(0, 3);

	switch(randomNumber) {
		case 0:
			return new MushroomItem(pos);
		case 1:
			return new ShieldItem(pos);
		case 2:
			return new SpeedItem(pos);
		default: //This is never reached
			return NULL;
	}
}