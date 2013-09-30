#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <game/character.h>
using namespace am::game;

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class BodyPartRenderer : public IEventListener, public UIComponent {
	public:
		BodyPartRenderer(short width, short height, const char *bodyPartName);
		~BodyPartRenderer();

		virtual void onEvent(EquipEvent *e);
		virtual void onEvent(MouseEvent *e);

		virtual void setMaxItemSize(short width, short height);
		virtual short getMaxItemWidth() const;
		virtual short getMaxItemHeight() const;

		virtual void setCharacter(Character *character);
		virtual Character *getCharacter() const;

		virtual void setBodyPartName(const char *partName);
		virtual const char *getBodyPartName() const;

		// Layer methods
		//virtual bool interactWithLayer() const;

		// Renderable methods
		virtual void preRender(float dt);

	protected:

		Handle<Character> mCharacter;
		Handle<Item> mCurrentItem;
		Handle<Renderable> mHitbox;

		string mBodyPartName;
		short mMaxItemWidth;
		short mMaxItemHeight;

		void updateGraphic();
	};

}
}
