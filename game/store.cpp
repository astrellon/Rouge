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

		buyerPurse->removeCoin(value);
		ownerPurse->addCoin(value);
		inventory->removeItem(item);
		return SUCCESS;
	}

	ReturnCode Store::sellItem(Character *seller, Item *item, bool makeNewInventories, bool allowZeroStoreCoin)
	{
		Handle<Item> itemHandle(item);
		if (!item || !seller)
		{
			return NULL_PARAMETER;
		}
		if (!mStoreOwner)
		{
			return NO_STORE_OWNER;
		}

		if (mStoreInventories.empty() && !makeNewInventories)
		{
			return NO_INVENTORIES;
		}

		CoinPurse *sellerPurse = seller->getCoinPurse();
		CoinPurse *ownerPurse = mStoreOwner->getCoinPurse();
		if (!sellerPurse || !ownerPurse)
		{
			return INTERNAL_ERROR;
		}

		int value = item->getItemValue();
		if (!allowZeroStoreCoin && ownerPurse->canRemoveCoin(value) < 0)
		{
			return NOT_ENOUGH_COIN;
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
				return NOT_ENOUGH_INVENTORY_SPACE;
			}
		}

		sellerPurse->addCoin(value);
		ownerPurse->removeCoin(value);
		inventory->addItem(item);

		return SUCCESS;
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
