#pragma once

#include <base/handle.h>
using namespace am::base;

#include "item.h"

namespace am {
namespace game {

	class InventorySpot {
	public:
		InventorySpot();
		InventorySpot(Item *item, unsigned short x, unsigned short y);
		~InventorySpot();

		Item *getItem() const;
		unsigned short getX() const;
		unsigned short getY() const;

		friend class Inventory;

	protected:
		Handle<Item> mItem;
		unsigned short mX, mY;

	};

}
}
