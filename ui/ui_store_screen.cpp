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
		mStore = store;
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
		if (index < 0 || index >= getNumStoreInventories())
		{
			return;
		}
		mStoreInventoryIndex = index;
		//mInventoryRenderer->setInventory(mStoreInventories[index].get());
	}
	int StoreScreen::getStoreInventoryIndex() const
	{
		return mStoreInventoryIndex;
	}
	int StoreScreen::getNumStoreInventories() const
	{
		//return static_cast<int>(mStoreInventories.size());
		return 1;
	}

	void StoreScreen::setWidth(float width)
	{
		Panel::setWidth(width);
	}
	void StoreScreen::setHeight(float height)
	{
		Panel::setHeight(height);
	}

	
}
}
