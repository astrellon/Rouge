#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_layer.h>
using namespace am::gfx;

#include "item.h"
#include "inventory_spot.h"

namespace am {
namespace game {

	class Inventory : public Layer {
	public:

		typedef vector< InventorySpot > InventorySpots;

		Inventory(unsigned short width, unsigned short height);
		~Inventory();

		unsigned short getSpacesX() const;
		unsigned short getSpacesY() const;

		bool hasSpaceFor(const Item *item) const;
		bool hasSpaceFor(const Item *item, int x, int y) const;
		bool addItem(Item *item);
		bool addItem(Item *item, int x, int y);
		bool removeItem(Item *item);
		bool removeAll();
		bool hasItem(Item *item);

		const InventorySpots &getInventory() const;

		void logContents() const;

		static void setSpaceSize(float sizeX, float sizeY);
		static float getSpaceSizeX();
		static float getSpaceSizeY();

	protected:
		
		unsigned short mSpacesX, mSpacesY;
		InventorySpots mSpots;

		Item **mSpotMap;

		int findItem(const Item *item) const;
		bool findSpotFor(const Item *, int &x, int &y) const;
		void placeItem(Item *item, int x, int y);

		static float sSpaceSizeX;
		static float sSpaceSizeY;

	};

}
}
