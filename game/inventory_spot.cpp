#include "inventory_spot.h"

namespace am {
namespace game {

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

}
}
