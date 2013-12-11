#pragma once

#include <base/handle.h>

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class MouseEvent;

	class Panel : public IEventListener, public UIComponent 
	{
	public:
		Panel();
		~Panel();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(MouseEvent *e);

		virtual gfx::TextField *getTitleField();
		virtual void setTitle(const char *title);
		virtual const char *getTitle() const;

		virtual gfx::Sprite *getBackSprite();

        virtual void show();
        virtual void hide();

	protected:

		float mStartingWidth;
		float mStartingHeight;
		bool mResizing;
		base::Handle<gfx::TextField> mTitle;
		base::Handle<gfx::Sprite> mBack;
		float mDragX;
		float mDragY;

	};

}
}
