#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <ui/event_interface.h>
using namespace am::ui;

#include "item.h"
#include "inventory_spot.h"

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;

	class Inventory : public virtual am::base::IManaged, public EventInterface {
	public:

		typedef vector< InventorySpot > InventorySpots;

		enum ItemMovementState {
			NOT_MOVING, ADDING, REMOVING
		};

		Inventory(unsigned short width, unsigned short height);
		Inventory(const Inventory &copy);
		~Inventory();

		unsigned short getSpacesX() const;
		unsigned short getSpacesY() const;

		bool hasSpaceFor(const Item *item) const;
		bool hasSpaceFor(const Item *item, int x, int y) const;
		bool addItem(Item *item);
		bool addItem(Item *item, int x, int y);
		bool removeItem(Item *item);
		bool removeAll();
		bool hasItem(const Item *item) const;

		ItemMovementState getItemMovementState() const;

		Item *getItemAt(int x, int y) const;

		const InventorySpots &getInventory() const;

		void logContents() const;

		data::IData *serialise();
		void deserialise(LoadingState *state, data::IData *data);

		static void setSpaceSize(float sizeX, float sizeY);
		static float getSpaceSizeX();
		static float getSpaceSizeY();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		unsigned short mSpacesX, mSpacesY;
		InventorySpots mSpots;

		Item **mSpotMap;

		void setSpaces(unsigned short width, unsigned short height);

		int findItem(const Item *item) const;
		bool findSpotFor(const Item *, int &x, int &y) const;
		bool placeItem(Item *item, int x, int y);

		ItemMovementState mItemMovementState;

		static float sSpaceSizeX;
		static float sSpaceSizeY;

	};

}
}
