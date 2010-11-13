#pragma once
#include "Item.h"

class ItemFactory {
	public:
		// Create an item at the position given by pos.
		// A random number decides which kind of item is created.
		Item* createItem(const Coord& pos);
};