#pragma once

#include <base/handle.h>

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>

#include <game/character.h>

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class BodyPartRenderer : public IEventListener, public UIComponent 
	{
	public:
		BodyPartRenderer(short width, short height, const char *bodyPartName);
		~BodyPartRenderer();

		virtual void onEvent(EquipEvent *e);
		virtual void onEvent(MouseEvent *e);

		virtual void setMaxItemSize(short width, short height);
		virtual short getMaxItemWidth() const;
		virtual short getMaxItemHeight() const;

		virtual void setCharacter(game::Character *character);
		virtual game::Character *getCharacter() const;

		virtual void setBodyPartName(const char *partName);
		virtual const char *getBodyPartName() const;

		// Renderable methods
		virtual void preRender(float dt);

	protected:

		base::Handle<game::Character> mCharacter;
		base::Handle<game::Item> mCurrentItem;
		base::Handle<gfx::Renderable> mHitbox;

		std::string mBodyPartName;
		short mMaxItemWidth;
		short mMaxItemHeight;

		void updateGraphic();
	};

}
}
