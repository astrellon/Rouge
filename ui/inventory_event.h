#pragma once

#include "event.h"
#include "inventory_event_common.h"

namespace am {
namespace game {
	class Inventory;
	class Item;
}

namespace ui {

	class InventoryEvent : public Event
	{
	public:
		InventoryEvent(Inventory::EventType type, game::Inventory *target);
		InventoryEvent(Inventory::EventType type, game::Inventory *target, game::Item *item);
		InventoryEvent(Inventory::EventType type, game::Inventory *target, game::Item *item,
			unsigned short spotX, unsigned short spotY);
		~InventoryEvent();

		virtual Inventory::EventType getInventoryEventType() const;
		virtual game::Inventory *getInventory() const;
		virtual game::Item *getItem() const;
		
		virtual unsigned short getSpotX() const;
		virtual unsigned short getSpotY() const;

		virtual void init();

	protected:

        Inventory::EventType mInventoryEventType;
        game::Inventory *mInventory;
        game::Item *mItem;

		unsigned short mSpotX;
		unsigned short mSpotY;

	};

}
}
