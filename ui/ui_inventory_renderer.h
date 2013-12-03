#pragma once

#include <base/handle.h>

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>

#include <game/inventory.h>

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class InventoryRenderer : public IEventListener, public UIComponent 
	{
	public:
		InventoryRenderer();
		~InventoryRenderer();

		virtual float getWidth();
		virtual float getHeight();

		virtual void onEvent(InventoryEvent *e);
		virtual void onEvent(MouseEvent *e);

		virtual game::Inventory *getInventory() const;
		virtual void setInventory(game::Inventory *inventory);

		// Renderable methods
		virtual void preRender(float dt);

	protected:

		base::Handle<game::Inventory> mInventory;
		base::Handle<gfx::Renderable> mHitbox;

		virtual void addItem(game::Item *item, int x, int y);
		virtual void removeItem(game::Item *item);

	};

}
}
