#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
#include <base/return_codes.h>
using namespace am::base;

#include <ui/event_interface.h>
using namespace am::ui;

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

		virtual ReturnCode buyItem(Character *buyer, Item *item);

		virtual bool addStoreInventory(Inventory *inventory);
		virtual bool removeStoreInventory(Inventory *inventory);
		virtual bool hasStoreInventory(Inventory *inventory) const;
		virtual void clearAllStoreInventories();
		virtual const InventoryList &getStoreInventories() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		Handle<Character> mStoreOwner;
		InventoryList mStoreInventories;

		virtual void addListeners(Inventory *inventory);
		virtual void removeListeners(Inventory *inventory);

	};

}
}
