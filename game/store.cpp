#include "store.h"

#include <ui/inventory_event.h>
using namespace am::ui;

#include <util/data_table.h>
#include <util/data_number.h>

#include <sstream>
#include <log/logger.h>

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int Store::LUA_ID = LUA_ID_STORE;
	const char *Store::LUA_TABLENAME = LUA_TABLE_STORE;

	Store::Store() :
		EventInterface()
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

	ReturnCode Store::buyItem(Character *buyer, Item *item)
	{
		Handle<Item> itemHandle(item);
		if (!item || !buyer)
		{
			return NULL_PARAMETER;
		}
		if (!mStoreOwner)
		{
			return NO_STORE_OWNER;
		}

		if (mStoreInventories.empty())
		{
			return NO_INVENTORIES;
		}

		CoinPurse *buyerPurse = buyer->getCoinPurse();
		CoinPurse *ownerPurse = mStoreOwner->getCoinPurse();
		if (!buyerPurse || !ownerPurse)
		{
			return INTERNAL_ERROR;
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
			return ITEM_NOT_IN_INVENTORY;
		}

		unsigned int value = item->getItemValue();
		if (buyerPurse->canRemoveCoin(value) < 0)
		{
			return NOT_ENOUGH_COIN;
		}
		if (!buyer->getInventory()->hasSpaceFor(item))
		{
			return NOT_ENOUGH_INVENTORY_SPACE;
		}

		buyerPurse->removeCoin(value);
		ownerPurse->addCoin(value);
		buyer->getInventory()->addItem(item);
		inventory->removeItem(item);
		return SUCCESS;
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
