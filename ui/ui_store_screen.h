#pragma once

#include <game/character.h>
#include <game/store.h>
using namespace am::game;

#include <base/handle.h>

#include <ui/ui_panel.h>
#include <ui/ui_inventory_renderer.h>

#include <vector>

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

		am::base::Handle<Character> mBuyer;
		am::base::Handle<Store> mStore;
		am::base::Handle<InventoryRenderer> mInventoryRenderer;

		int mStoreInventoryIndex;

		virtual void addListeners(Store *store);
		virtual void removeListeners(Store *store);
	};

}
}