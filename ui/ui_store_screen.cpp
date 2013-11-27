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
		setStore(nullptr);
		clear();
	}

	void StoreScreen::setStore(Store *store)
	{
		if (mStore)
		{
			removeListeners(mStore);
		}
		mStore = store;
		if (mStore)
		{
			addListeners(mStore);
		}
		setStoreInventoryIndex(0);
	}
	Store *StoreScreen::getStore() const
	{
		return mStore;
	}

	void StoreScreen::onEvent(InventoryEvent *e)
	{
		if (!e)
		{
			return;
		}
		Item *item = e->getItem();
		if (!item)
		{
			return;
		}
		/*if (mBuyer && mStore)
		{
			unsigned int value = item->getItemValue();
			CoinPurse *buyerPurse = mBuyer->getCoinPurse();
			if (!buyerPurse)
			{
				am_log("STORE", "Buyer does not have a purse!");
				return;
			}
			CoinPurse *ownerPurse = mStore->getStoreOwner()->getCoinPurse();
			if (!ownerPurse)
			{
				am_log("STORE", "Owner does not have a purse!");
				return;
			}
		}*/
		// CHANGE THIS, 
		if (e->getInventoryEventType() == INVENTORY_BEFORE_REMOVE)
		{
			base::ReturnCode result = mStore->buyItem(mBuyer, e->getItem(), 1.5f);
			am_log("BUY", getErrorName(result));
			if (result != base::SUCCESS)
			{
				e->stopPropagation();
			}
		}
		else if (e->getInventoryEventType() == INVENTORY_BEFORE_ADD)
		{
			base::ReturnCode result = mStore->sellItem(mBuyer, e->getItem(), 0.75f, true, true);
			am_log("SELL", getErrorName(result));
			if (result != base::SUCCESS)
			{
				e->stopPropagation();
			}
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

	

	void StoreScreen::setStoreInventoryIndex(int index)
	{
		if (index < 0 || index >= getNumStoreInventories() || !mStore)
		{
			return;
		}
		mStoreInventoryIndex = index;
		mInventoryRenderer->setInventory(mStore->getStoreInventories()[index]);
	}
	int StoreScreen::getStoreInventoryIndex() const
	{
		return mStoreInventoryIndex;
	}
	int StoreScreen::getNumStoreInventories() const
	{
		if (mStore)
		{
			return mStore->getNumStoreInventories();
		}
		return 0;
	}

	void StoreScreen::setWidth(float width)
	{
		Panel::setWidth(width);
	}
	void StoreScreen::setHeight(float height)
	{
		Panel::setHeight(height);
	}

	void StoreScreen::addListeners(Store *store)
	{
		if (store)
		{
			store->addEventListener(InventoryEventTypeName[INVENTORY_ADD], this);
			store->addEventListener(InventoryEventTypeName[INVENTORY_REMOVE], this);
			store->addEventListener(InventoryEventTypeName[INVENTORY_BEFORE_ADD], this);
			store->addEventListener(InventoryEventTypeName[INVENTORY_BEFORE_REMOVE], this);
		}
	}
	void StoreScreen::removeListeners(Store *store)
	{
		if (store)
		{
			store->removeEventListener(InventoryEventTypeName[INVENTORY_ADD], this);
			store->removeEventListener(InventoryEventTypeName[INVENTORY_REMOVE], this);
			store->removeEventListener(InventoryEventTypeName[INVENTORY_BEFORE_ADD], this);
			store->removeEventListener(InventoryEventTypeName[INVENTORY_BEFORE_REMOVE], this);
		}
	}

}
}
