#pragma once

#include <game/character.h>
#include <game/inventory.h>
using namespace am::game;

#include <base/handle.h>
using namespace am::base;

#include <ui/ui_panel.h>
#include <ui/ui_inventory_renderer.h>

#include <vector>
using namespace std;

namespace am {
namespace ui {

	class StoreScreen : public Panel {
	public:
		StoreScreen();
		~StoreScreen();

		typedef vector< Handle<Inventory> > InventoryList;

		virtual void setBuyer(Character *character);
		virtual Character *getBuyer() const;

		virtual void setStoreOwner(Character *character);
		virtual Character *getStoreOwner() const;

		virtual bool addStoreInventory(Inventory *inventory);
		virtual bool removeStoreInventory(Inventory *inventory);
		virtual bool hasStoreInventory(Inventory *inventory) const;
		virtual void clearAllStoreInventories();
		virtual const InventoryList &getStoreInventories() const;

		virtual void setStoreInventoryIndex(int index);
		virtual int getStoreInventoryIndex() const;
		virtual int getNumStoreInventories() const;

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(InventoryEvent *e);

	protected:

		Handle<Character> mBuyer;
		Handle<Character> mStoreOwner;
		Handle<InventoryRenderer> mInventoryRenderer;

		InventoryList mStoreInventories;
		int mStoreInventoryIndex;

		virtual void addListeners(Inventory *inventory);
		virtual void removeListeners(Inventory *inventory);
	};

}
}
