#include "item_instance.h"

namespace am {
namespace game {

	ItemInstance::ItemInstance()
	{
	}
	ItemInstance::ItemInstance(Item *item) :
		mItem(item)
	{
	}
	ItemInstance::~ItemInstance()
	{
	}

	void ItemInstance::setItem(Item *item)
	{
		mItem = item;
	}
	Item *ItemInstance::getItem()
	{
		return mItem.get();
	}

	void ItemInstance::setInventoryPosition(short x, short y)
	{
		mInventoryPosX = x;
		mInventoryPosY = y;
	}
	short ItemInstance::getInventoryPositionX() const
	{
		return mInventoryPosX;
	}
	short ItemInstance::getInventoryPositionY() const
	{
		return mInventoryPosY;
	}

	void ItemInstance::update(float dt)
	{

	}

}
}
