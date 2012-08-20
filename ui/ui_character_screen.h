#pragma once

#include <game/character.h>
using namespace am::game;

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include <ui/ui_panel.h>
#include <ui/ui_inventory_renderer.h>

namespace am {
namespace ui {

	class CharacterScreen : public Panel {
	public:
		CharacterScreen();
		~CharacterScreen();

		virtual TextField2 *getTextField();

		virtual void setCharacter(Character *character);
		virtual Character *getCharacter();

		virtual void setTextDirty();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(EquipEvent *e);

		virtual void render(float dt);

	protected:

		Handle<TextField2> mInfo;
		Handle<Character> mCharacter;
		Handle<InventoryRenderer> mInventory;

		bool mTextDirty;
		void updateText();
	};

}
}
