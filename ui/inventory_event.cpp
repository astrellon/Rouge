#include "inventory_event.h"

#include <game/inventory.h>
#include <game/item.h>

namespace am {
namespace ui {

	InventoryEvent::InventoryEvent( Inventory::EventType type, game::Inventory *target ) :
		Event(Inventory::EventTypeName[type]),
		mInventoryEventType(type),
		mInventory(target),
		mSpotX(0),
		mSpotY(0)
	{
		init();
	}

	InventoryEvent::InventoryEvent( Inventory::EventType type, game::Inventory *target, game::Item *item ) :
		Event(Inventory::EventTypeName[type]),
		mInventoryEventType(type),
		mInventory(target),
		mItem(item),
		mSpotX(0),
		mSpotY(0)
	{
		init();
	}

	InventoryEvent::InventoryEvent( Inventory::EventType type, game::Inventory *target, game::Item *item, unsigned short spotX, unsigned short spotY ) :
		Event(Inventory::EventTypeName[type]),
		mInventoryEventType(type),
		mInventory(target),
		mItem(item),
		mSpotX(spotX),
		mSpotY(spotY)
	{
		init();
	}

	InventoryEvent::~InventoryEvent()
	{
		if (mInventory)
		{
			mInventory->release();
		}
		if (mItem)
		{
			mItem->release();
		}
	}

	void InventoryEvent::init()
	{
		if (mInventory)
		{
			mInventory->retain();
		}
		if (mItem)
		{
			mItem->retain();
		}
	}

	Inventory::EventType InventoryEvent::getInventoryEventType() const
	{
		return mInventoryEventType;
	}
	game::Inventory *InventoryEvent::getInventory() const
	{
		return mInventory;
	}

	game::Item *InventoryEvent::getItem() const
	{
		return mItem;
	}

	unsigned short InventoryEvent::getSpotX() const
	{
		return mSpotX;
	}
	unsigned short InventoryEvent::getSpotY() const
	{
		return mSpotY;
	}

}
}
