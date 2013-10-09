#include "ui_store_screen.h"

#include <sstream>

namespace am {
namespace ui {

	StoreScreen::StoreScreen() :
		Panel(),
		mStoreInventoryIndex(-1)
	{
		setName("StoreScreen");
		
		mInventoryRenderer = new InventoryRenderer();
		mInventoryRenderer->setName("InventoryRenderer");
		addChild(mInventoryRenderer);
		mInventoryRenderer->setOffset(-16.0f, 16.0f);
		mInventoryRenderer->setAnchorY(Y_BOTTOM);
		mInventoryRenderer->setParentAnchorY(Y_BOTTOM);

		setInteractive(true);
		mInventoryRenderer->setInteractive(true);

		setTitle("Store Screen");
	}
	StoreScreen::~StoreScreen()
	{
		setBuyer(nullptr);
		setStoreOwner(nullptr);
		clear();
	}

	void StoreScreen::onEvent(InventoryEvent *e)
	{
		if (!e)
		{
			return;
		}
		am_log("STORE", e->getType());
	}

	void StoreScreen::setBuyer(Character *character)
	{
		if (character != mBuyer)
		{
			mBuyer = character;
		}
	}
	Character *StoreScreen::getBuyer() const
	{
		return mBuyer;
	}

	void StoreScreen::setStoreOwner(Character *character)
	{
		if (character != mStoreOwner)
		{
			mStoreOwner = character;
		}
	}
	Character *StoreScreen::getStoreOwner() const
	{
		return mStoreOwner;
	}

	bool StoreScreen::addStoreInventory(Inventory *inventory)
	{
		if (!inventory || hasStoreInventory(inventory))
		{
			return false;
		}
		mStoreInventories.push_back(inventory);
		addListeners(inventory);
		if (mStoreInventoryIndex < 0)
		{
			setStoreInventoryIndex(0);
		}
		return true;
	}
	bool StoreScreen::removeStoreInventory(Inventory *inventory)
	{
		if (!inventory)
		{
			return false;
		}
		for (auto iter = mStoreInventories.begin(); iter != mStoreInventories.end(); ++iter)
		{
			if (iter->get() == inventory)
			{
				removeListeners(inventory);
				mStoreInventories.erase(iter);
				if (mStoreInventoryIndex >= getNumStoreInventories())
				{
					mStoreInventoryIndex = getNumStoreInventories() - 1;
				}
				return true;
			}
		}
		return false;
	}
	bool StoreScreen::hasStoreInventory(Inventory *inventory) const
	{
		if (!inventory)
		{
			return false;
		}
		for (auto iter = mStoreInventories.begin(); iter != mStoreInventories.end(); ++iter)
		{
			if (iter->get() == inventory)
			{
				return true;
			}
		}
		return false;
	}
	void StoreScreen::clearAllStoreInventories()
	{
		mStoreInventoryIndex = -1;
		for (auto iter = mStoreInventories.begin(); iter != mStoreInventories.end(); ++iter)
		{
			removeListeners(iter->get());
		}
		mStoreInventories.clear();
		mInventoryRenderer->setInventory(nullptr);
	}
	const StoreScreen::InventoryList &StoreScreen::getStoreInventories() const
	{
		return mStoreInventories;
	}

	void StoreScreen::setStoreInventoryIndex(int index)
	{
		if (index < 0 || index >= getNumStoreInventories())
		{
			return;
		}
		mStoreInventoryIndex = index;
		mInventoryRenderer->setInventory(mStoreInventories[index].get());
	}
	int StoreScreen::getStoreInventoryIndex() const
	{
		return mStoreInventoryIndex;
	}
	int StoreScreen::getNumStoreInventories() const
	{
		return static_cast<int>(mStoreInventories.size());
	}

	void StoreScreen::setWidth(float width)
	{
		Panel::setWidth(width);
	}
	void StoreScreen::setHeight(float height)
	{
		Panel::setHeight(height);
	}

	void StoreScreen::addListeners(Inventory *inventory)
	{
		if (inventory)
		{
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_ADD], this);
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_REMOVE], this);
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_BEFORE_ADD], this);
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_BEFORE_REMOVE], this);
		}
	}
	void StoreScreen::removeListeners(Inventory *inventory)
	{
		if (inventory)
		{
			inventory->removeEventListener(InventoryEventTypeName[INVENTORY_ADD], this);
			inventory->removeEventListener(InventoryEventTypeName[INVENTORY_REMOVE], this);
			inventory->removeEventListener(InventoryEventTypeName[INVENTORY_BEFORE_ADD], this);
			inventory->removeEventListener(InventoryEventTypeName[INVENTORY_BEFORE_REMOVE], this);
		}
	}
}
}
