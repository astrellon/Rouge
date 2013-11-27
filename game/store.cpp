#include "store.h"

#include <ui/inventory_event.h>
using namespace am::ui;

#include <util/data_table.h>
#include <util/data_number.h>

#include <math/math.h>

#include <sstream>
#include <log/logger.h>

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int Store::LUA_ID = LUA_ID_STORE;
	const char *Store::LUA_TABLENAME = LUA_TABLE_STORE;

	Store::Store() :
		EventInterface(),
		mDefaultInventorySize(20, 16)
	{
	}
	Store::~Store()
	{
	}

	void Store::setStoreOwner(Character *character)
	{
		if (character != mStoreOwner)
		{
			mStoreOwner = character;
		}
	}
	Character *Store::getStoreOwner() const
	{
		return mStoreOwner;
	}

	am::base::ReturnCode Store::buyItem(Character *buyer, Item *item, float valueFactor)
	{
		am::base::Handle<Item> itemHandle(item);
		if (!item || !buyer)
		{
			return am::base::NULL_PARAMETER;
		}
		if (!mStoreOwner)
		{
			return am::base::NO_STORE_OWNER;
		}

		if (mStoreInventories.empty())
		{
			return am::base::NO_INVENTORIES;
		}

		CoinPurse *buyerPurse = buyer->getCoinPurse();
		CoinPurse *ownerPurse = mStoreOwner->getCoinPurse();
		if (!buyerPurse || !ownerPurse)
		{
			return am::base::INTERNAL_ERROR;
		}
		
		Inventory *inventory = nullptr;
		for (auto iter = mStoreInventories.begin(); iter != mStoreInventories.end(); ++iter)
		{
			if (iter->get()->hasItem(item))
			{
				inventory = iter->get();
				break;
			}
		}
		if (!inventory)
		{
			return am::base::ITEM_NOT_IN_INVENTORY;
		}

		int value = math::round(static_cast<float>(item->getItemValue()) * valueFactor);
		if (buyerPurse->canRemoveCoin(value) < 0)
		{
			return am::base::NOT_ENOUGH_COIN;
		}

		buyerPurse->removeCoin(value);
		ownerPurse->addCoin(value);
		inventory->removeItem(item);
		return am::base::SUCCESS;
	}

	am::base::ReturnCode Store::sellItem(Character *seller, Item *item, float valueFactor, bool makeNewInventories, bool allowZeroStoreCoin)
	{
		am::base::Handle<Item> itemHandle(item);
		if (!item || !seller)
		{
			return am::base::NULL_PARAMETER;
		}
		if (!mStoreOwner)
		{
			return am::base::NO_STORE_OWNER;
		}

		if (mStoreInventories.empty() && !makeNewInventories)
		{
			return am::base::NO_INVENTORIES;
		}

		CoinPurse *sellerPurse = seller->getCoinPurse();
		CoinPurse *ownerPurse = mStoreOwner->getCoinPurse();
		if (!sellerPurse || !ownerPurse)
		{
			return am::base::INTERNAL_ERROR;
		}

		int value = math::round(static_cast<float>(item->getItemValue()) * valueFactor);
		if (!allowZeroStoreCoin && ownerPurse->canRemoveCoin(value) < 0)
		{
			return am::base::NOT_ENOUGH_COIN;
		}
		
		Inventory *inventory = nullptr;
		for (auto iter = mStoreInventories.begin(); iter != mStoreInventories.end(); ++iter)
		{
			if (iter->get()->hasSpaceFor(item))
			{
				inventory = iter->get();
				break;
			}
		}
		if (!inventory)
		{
			if (makeNewInventories)
			{
				inventory = createStoreInventory();
			}
			else
			{
				return am::base::NOT_ENOUGH_INVENTORY_SPACE;
			}
		}

		sellerPurse->addCoin(value);
		ownerPurse->removeCoin(value);
		inventory->addItem(item);

		return am::base::SUCCESS;
	}

	Inventory *Store::createStoreInventory()
	{
		Inventory *inv = new Inventory(mDefaultInventorySize.x, mDefaultInventorySize.y);
		if (addStoreInventory(inv))
		{
			return inv;
		}
		return nullptr;
	}
	bool Store::addStoreInventory(Inventory *inventory)
	{
		if (!inventory || hasStoreInventory(inventory))
		{
			return false;
		}
		mStoreInventories.push_back(inventory);
		addListeners(inventory);
		return true;
	}
	bool Store::removeStoreInventory(Inventory *inventory)
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
				return true;
			}
		}
		return false;
	}
	bool Store::hasStoreInventory(Inventory *inventory) const
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
	void Store::clearAllStoreInventories()
	{
		for (auto iter = mStoreInventories.begin(); iter != mStoreInventories.end(); ++iter)
		{
			removeListeners(iter->get());
		}
		mStoreInventories.clear();
	}
	const Store::InventoryList &Store::getStoreInventories() const
	{
		return mStoreInventories;
	}
	int Store::getNumStoreInventories() const
	{
		return static_cast<int>(mStoreInventories.size());
	}

	void Store::setDefaultInventorySize(int width, int height)
	{
		mDefaultInventorySize.x = width;
		mDefaultInventorySize.y = height;
	}
	unsigned short Store::getDefaultInventorySizeWidth() const
	{
		return mDefaultInventorySize.x;
	}
	unsigned short Store::getDefaultInventorySizeHeight() const
	{
		return mDefaultInventorySize.y;
	}
	Vector2<unsigned short> Store::getDefaultInventorySize() const
	{
		return mDefaultInventorySize;
	}

	void Store::onEvent(InventoryEvent *e)
	{
		if (!e)
		{
			return;
		}
		fireEvent<InventoryEvent>(e);
	}

	void Store::addListeners(Inventory *inventory)
	{
		if (inventory)
		{
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_ADD], this);
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_REMOVE], this);
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_BEFORE_ADD], this);
			inventory->addEventListener(InventoryEventTypeName[INVENTORY_BEFORE_REMOVE], this);
		}
	}
	void Store::removeListeners(Inventory *inventory)
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