#pragma once

#include <game/character.h>
#include <game/store.h>
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

		virtual void setBuyer(Character *character);
		virtual Character *getBuyer() const;

		virtual void setStore(Store *store);
		virtual Store *getStore() const;

		virtual void setStoreInventoryIndex(int index);
		virtual int getStoreInventoryIndex() const;
		virtual int getNumStoreInventories() const;

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(InventoryEvent *e);

	protected:

		Handle<Character> mBuyer;
		Handle<Store> mStore;
		Handle<InventoryRenderer> mInventoryRenderer;

		int mStoreInventoryIndex;

		virtual void addListeners(Store *store);
		virtual void removeListeners(Store *store);
	};

}
}
