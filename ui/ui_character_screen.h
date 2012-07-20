#pragma once

#include <game/character.h>
using namespace am::game;

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field.h>
using namespace am::gfx;

#include <ui/ui_panel.h>

namespace am {
namespace ui {

	class CharacterScreen : public Panel {
	public:
		CharacterScreen();
		~CharacterScreen();

		virtual TextField *getTextField();

		virtual void setCharacter(Character *character);
		virtual Character *getCharacter();

		virtual void setTextDirty();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void render(float dt);

	protected:

		Handle<TextField> mInfo;
		Handle<Character> mCharacter;

		bool mTextDirty;
		void updateText();
	};

}
}
