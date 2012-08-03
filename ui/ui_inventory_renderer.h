#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <game/inventory.h>
using namespace am::game;

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class InventoryRenderer : public IEventListener, public UIComponent {
	public:
		InventoryRenderer();
		~InventoryRenderer();

		virtual float getWidth();
		virtual float getHeight();

		virtual void onEvent(InventoryEvent *e);

		virtual Inventory *getInventory() const;
		virtual void setInventory(Inventory *inventory);

		virtual void preRender(float dt);

	protected:

		Handle<Inventory> mInventory;

		virtual void addItem(Item *item, int x, int y);

	};

}
}
