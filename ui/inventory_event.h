#pragma once

#include "event.h"
#include "inventory_event_common.h"

namespace am {
namespace game {
	class Inventory;
	class Item;
}

namespace ui {

	class InventoryEvent : public Event {
	public:
		InventoryEvent(am::ui::Inventory::EventType type, game::Inventory *target);
		InventoryEvent(am::ui::Inventory::EventType type, game::Inventory *target, game::Item *item);
		InventoryEvent(am::ui::Inventory::EventType type, game::Inventory *target, game::Item *item,
			unsigned short spotX, unsigned short spotY);
		~InventoryEvent();

		virtual am::ui::Inventory::EventType getInventoryEventType() const;
		virtual game::Inventory *getInventory() const;
		virtual game::Item *getItem() const;
		
		virtual unsigned short getSpotX() const;
		virtual unsigned short getSpotY() const;

		virtual void init();

	protected:

        am::ui::Inventory::EventType mInventoryEventType;
        game::Inventory *mInventory;
        game::Item *mItem;

		unsigned short mSpotX;
		unsigned short mSpotY;

	};

}
}
