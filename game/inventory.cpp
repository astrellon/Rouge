#include "inventory.h"

#include <ui/inventory_event.h>
using namespace am::ui;

#include <sstream>
#include <log/logger.h>

namespace am {
namespace game {

	float Inventory::sSpaceSizeX = 16.0f;
	float Inventory::sSpaceSizeY = 16.0f;

	const int Inventory::LUA_ID = 0x09;
	const char *Inventory::LUA_TABLENAME = "am_game_Inventory";

	Inventory::Inventory(unsigned short width, unsigned short height) :
		EventInterface(),
		mSpacesX(width),
		mSpacesY(height)
	{
		int size = width * height;
		mSpotMap = new Item *[size];
		for (int i = 0; i < size; i++)
		{
			mSpotMap[i] = NULL;
		}
	}

	Inventory::~Inventory()
	{
		delete mSpotMap;
	}

	unsigned short Inventory::getSpacesX() const
	{
		return mSpacesX;
	}
	unsigned short Inventory::getSpacesY() const
	{
		return mSpacesY;
	}

	bool Inventory::hasSpaceFor(const Item *item) const
	{
		int x = 0;
		int y = 0;
		return findSpotFor(item, x, y);
	}
	bool Inventory::hasSpaceFor(const Item *item, int x, int y) const
	{
		if (item == NULL || x < 0 || y < 0)
		{
			return false;
		}
		int xEnd = item->getInventorySizeX() + x;
		int yEnd = item->getInventorySizeY() + y;
		if (xEnd > mSpacesX || yEnd > mSpacesY)
		{
			return false;
		}

		for (int j = y; j < yEnd; j++)
		{
			int jOffset = j * mSpacesX;
			for (int i = x; i < xEnd; i++)
			{
				if (mSpotMap[i + jOffset] != NULL)
				{
					return false;
				}
			}
		}
		
		return true;
	}
	bool Inventory::addItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}
		if (hasItem(item))
		{
			return true;
		}

		int x = 0;
		int y = 0;
		if (findSpotFor(item, x, y))
		{
			placeItem(item, x, y);

			return true;
		}
		return false;
	}
	bool Inventory::addItem(Item *item, int x, int y)
	{
		if (item == NULL)
		{
			return false;
		}
		if (hasItem(item))
		{
			return true;
		}
		if (hasSpaceFor(item, x, y))
		{
			placeItem(item, x, y);

			return true;
		}
		return false;
	}

	void Inventory::placeItem(Item *item, int x, int y)
	{
		mSpots.push_back(InventorySpot(item, x, y));
		int xEnd = x + item->getInventorySizeX();
		int yEnd = y + item->getInventorySizeY();

		item->setMap(NULL);

		for (int j = y; j < yEnd; j++)
		{
			int jOffset = j * mSpacesX;
			for (int i = x; i < xEnd; i++)
			{
				mSpotMap[i + jOffset] = item;
			}
		}

		//addChild(item);
		item->setItemLocation(Item::INVENTORY);
		//item->setPosition(x * Inventory::getSpaceSizeX(), y * Inventory::getSpaceSizeY());
		fireEvent<InventoryEvent>(new InventoryEvent(INVENTORY_ADD, this, item, x, y));
	}
	bool Inventory::removeItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}
		
		int index = findItem(item);
		if (index < 0)
		{
			return false;
		}

		InventorySpot &spot = mSpots[index];
		int xEnd = spot.getX() + spot.getItem()->getInventorySizeX();
		int yEnd = spot.getY() + spot.getItem()->getInventorySizeY();

		for (int j = spot.getY(); j < yEnd; j++)
		{
			int jOffset = j * mSpacesX;
			for (int i = spot.getX(); i < xEnd; i++)
			{
				mSpotMap[i + jOffset] = NULL;
			}
		}
		// Keep items alive while event fires.
		Handle<Item> itemHandle(item);
		mSpots.erase(mSpots.begin() + index);

		item->setItemLocation(Item::GROUND);
		fireEvent<InventoryEvent>(new InventoryEvent(INVENTORY_REMOVE, this, item, spot.getX(), spot.getY()));

		return true;
	}
	bool Inventory::removeAll()
	{
		while (mSpots.size())
		{
			if (!removeItem(mSpots[0].getItem()))
			{
				return false;
			}
		}
		return true;
	}
	bool Inventory::hasItem(const Item *item) const
	{
		return findItem(item) >= 0;
	}

	Item *Inventory::getItemAt(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= mSpacesX || y >= mSpacesY)
		{
			return NULL;
		}
		return mSpotMap[y * mSpacesX + x];
	}

	const Inventory::InventorySpots &Inventory::getInventory() const
	{
		return mSpots;
	}

	int Inventory::findItem(const Item *item) const
	{
		for (size_t i = 0; i < mSpots.size(); i++)
		{
			if (mSpots[i].getItem() == item)
			{
				return static_cast<int>(i);
			}
		}
		return -1;
	}
	bool Inventory::findSpotFor(const Item *item, int &x, int &y) const
	{
		bool found = false;
		for (int j = 0; j < mSpacesY; j++)
		{
			int jOffset = j * mSpacesX;
			for (int i = 0; i < mSpacesX; i++)
			{
				if (mSpotMap[i + jOffset] != NULL)
				{
					continue;
				}

				if (hasSpaceFor(item, i, j))
				{
					x = i;
					y = j;
					found = true;
					break;
				}
			}
			if (found)
			{
				break;
			}
		}
		return found;
	}

	void Inventory::setSpaceSize(float sizeX, float sizeY)
	{
		if (sizeX < 1.0f)
		{
			sizeX = 1.0f;
		}
		if (sizeY < 1.0f)
		{
			sizeY = 1.0f;
		}
		sSpaceSizeX = sizeX;
		sSpaceSizeY = sizeY;
	}

	float Inventory::getSpaceSizeX()
	{
		return sSpaceSizeX;
	}

	float Inventory::getSpaceSizeY()
	{
		return sSpaceSizeY;
	}

	void Inventory::logContents() const
	{
		if (mSpots.size() == 0)
		{
			am_log("INV", "Inventory empty");
			return;
		}
		for (size_t i = 0; i < mSpots.size(); i++)
		{
			stringstream ss;
			const InventorySpot &spot = mSpots[i];
			ss << "Inv " << i << ": " << spot.getItem()->getFullItemName();
			ss << " [" << spot.getItem()->getItemTypeName() << "]";
			ss << " @(" << spot.getX() << ", " << spot.getY() << ")";
			am_log("INV", ss);

		}
	}

}
}
