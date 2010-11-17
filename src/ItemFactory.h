#pragma once
#include "Item.h"
#include "shared_ptr.h"

class ItemFactory {
	public:
		// Create an item at the position given by pos.
		// A random number decides which kind of item is created.
		shared_ptr<Item> createItem(const Coord& pos);
};
