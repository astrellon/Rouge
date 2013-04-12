#include "inventory_spot.h"

#include <util/data_map.h>

namespace am {
namespace game {

	const int InventorySpot::LUA_ID = 0x0A;
	const char *InventorySpot::LUA_TABLENAME = "am_game_InventorySpot";

	InventorySpot::InventorySpot() :
		mX(0),
		mY(0)
	{

	}
	InventorySpot::InventorySpot(Item *item, unsigned short x, unsigned short y) :
		mItem(item),
		mX(x),
		mY(y)
	{

	}
	InventorySpot::~InventorySpot()
	{

	}

	Item *InventorySpot::getItem() const
	{
		return mItem;
	}
	unsigned short InventorySpot::getX() const
	{
		return mX;
	}
	unsigned short InventorySpot::getY() const
	{
		return mY;
	}

	data::IData *InventorySpot::getSaveObject()
	{
		data::Map *output = new data::Map();
		output->push("x", mX);
		output->push("y", mY);
		if (mItem)
		{
			output->push("item", mItem->getSaveObject());
		}
		return output;
	}

}
}
