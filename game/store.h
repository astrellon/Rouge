#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
#include <base/return_codes.h>
using namespace am::base;

#include <ui/event_interface.h>
using namespace am::ui;

#include <math/vector.h>
using namespace am::math;

#include "character.h"
#include "inventory.h"

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class Store : public virtual IManaged, public EventInterface, public IEventListener {
	public:

		Store();
		~Store();

		typedef vector< Handle<Inventory> > InventoryList;

		virtual void setStoreOwner(Character *character);
		virtual Character *getStoreOwner() const;

		/**
		 * Attempts to buy the item.
		 * Bought item will be removed from the store inventories
		 * but is not added to the buyers inventory.
		 *
		 * Return codes:
		 *  NULL_PARAMETER: Either buyer or item was null.
		 *  NO_STORE_OWNER: No store owner assigned to this store.
		 *  NO_INVENTORIES: Cannot buy because no inventories in this store.
		 *  INTERNAL_ERROR: Either store owner or buyer does not have a coin purse.
		 *  ITEM_NOT_IN_INVENTORY: Cannot buy the item because it's not in any of the store inventories. 
		 *  NOT_ENOUGH_COIN: Buyer does not have enough coin to buy the item.
		 *  SUCCESS: Item was bought successfully.
		 */
		virtual ReturnCode buyItem(Character *buyer, Item *item);
		/**
		 * Attempts to sell the item.
		 * Sold item is added to the store inventory and is not removed
		 * from its current location.
		 *
		 * Return codes:
		 *  NULL_PARAMETER: Either seller or item was null.
		 *  NO_STORE_OWNER: No store owner assigned to this store.
		 *  NO_INVENTORIES: If makeNewInventories is false and there are no inventories in this store.
		 *  INTERNAL_ERROR: Either store owner or seller does not have a coin purse.
		 *  NOT_ENOUGH_INVENTORY_SPACE: If makeNewInventories is false and the current inventories are full.
		 *  NOT_ENOUGH_COIN: If allowZeroStoreCoin is false and the store owner does not have enough to pay for the item.
		 *  SUCCESS: Item was sold successfully.
		 */
		virtual ReturnCode sellItem(Character *seller, Item *item, bool makeNewInventories = false, bool allowZeroStoreCoin = false);

		virtual Inventory *createStoreInventory();
		virtual bool addStoreInventory(Inventory *inventory);
		virtual bool removeStoreInventory(Inventory *inventory);
		virtual bool hasStoreInventory(Inventory *inventory) const;
		virtual void clearAllStoreInventories();
		virtual const InventoryList &getStoreInventories() const;
		virtual int getNumStoreInventories() const;

		virtual void setDefaultInventorySize(int width, int height);
		virtual unsigned short getDefaultInventorySizeWidth() const;
		virtual unsigned short getDefaultInventorySizeHeight() const;
		virtual Vector2<unsigned short> getDefaultInventorySize() const;

		virtual void onEvent(InventoryEvent *e);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		Handle<Character> mStoreOwner;
		InventoryList mStoreInventories;

		Vector2<unsigned short> mDefaultInventorySize;

		virtual void addListeners(Inventory *inventory);
		virtual void removeListeners(Inventory *inventory);

	};

}
}
