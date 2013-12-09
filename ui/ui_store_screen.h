#pragma once

#include <game/character.h>
#include <game/store.h>

#include <base/handle.h>

#include <ui/ui_panel.h>
#include <ui/ui_inventory_renderer.h>

#include <vector>

namespace am {
namespace ui {

	class StoreScreen : public Panel 
	{
	public:
		StoreScreen();
		~StoreScreen();

		virtual void setBuyer(game::Character *character);
		virtual game::Character *getBuyer() const;

		virtual void setStore(game::Store *store);
		virtual game::Store *getStore() const;

		virtual void setStoreInventoryIndex(int index);
		virtual int getStoreInventoryIndex() const;
		virtual int getNumStoreInventories() const;

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(InventoryEvent *e);

	protected:

		base::Handle<game::Character> mBuyer;
		base::Handle<game::Store> mStore;
		base::Handle<InventoryRenderer> mInventoryRenderer;

		int mStoreInventoryIndex;

		virtual void addListeners(game::Store *store);
		virtual void removeListeners(game::Store *store);
	};

}
}

