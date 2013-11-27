#pragma once

#include <game/character.h>
using namespace am::game;

#include <base/handle.h>

#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include <ui/ui_panel.h>
#include <ui/ui_inventory_renderer.h>
#include <ui/ui_body_part_renderer.h>

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

		virtual void onEvent(Event *e);
		virtual void onEvent(EquipEvent *e);
		virtual void onEvent(StatEvent *e);

		virtual void render(float dt);

	protected:

		base::Handle<TextField2> mInfo;
		base::Handle<Character> mCharacter;
		base::Handle<InventoryRenderer> mInventory;
		base::Handle<BodyPartRenderer> mLeftHand;
		base::Handle<BodyPartRenderer> mRightHand;
		base::Handle<BodyPartRenderer> mTorso;
		base::Handle<BodyPartRenderer> mHead;
		base::Handle<BodyPartRenderer> mLegs;

		bool mTextDirty;
		void updateText();
	};

}
}
