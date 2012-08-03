#pragma once

#include "event.h"
#include "inventory_event_common.h"

namespace am {
namespace game {
	class Inventory;
	class Item;
}
using namespace am::game;

namespace ui {

	class InventoryEvent : public Event {
	public:
		InventoryEvent(InventoryEventType type, Inventory *target);
		InventoryEvent(InventoryEventType type, Inventory *target, Item *item);
		InventoryEvent(InventoryEventType type, Inventory *target, Item *item,
			unsigned short spotX, unsigned short spotY);
		~InventoryEvent();

		virtual InventoryEventType getInventoryEventType() const;
		virtual Inventory *getInventory() const;
		virtual Item *getItem() const;
		
		virtual unsigned short getSpotX() const;
		virtual unsigned short getSpotY() const;

		virtual void init();

	protected:

		InventoryEventType mInventoryEventType;
		Inventory *mInventory;
		Item *mItem;

		unsigned short mSpotX;
		unsigned short mSpotY;

	};

}
}
