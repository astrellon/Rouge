#pragma once

#include <base/handle.h>

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class MouseEvent;
    class TextButton;

	class Panel : public IEventListener, public UIComponent 
	{
	public:
		Panel();
		~Panel();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

        virtual void onEvent(Event *e);
		virtual void onEvent(MouseEvent *e);

		virtual gfx::TextField *getTitleField() const;
		virtual void setTitle(const char *title);
		virtual const char *getTitle() const;

		virtual gfx::Sprite *getBackSprite() const;
        virtual TextButton *getCloseButton() const;

        virtual void show();
        virtual void hide();

	protected:

		float mStartingWidth;
		float mStartingHeight;
		bool mResizing;
		base::Handle<gfx::TextField> mTitle;
		base::Handle<gfx::Sprite> mBack;
        TextButton *mCloseButton;
		float mDragX;
		float mDragY;

	};

}
}
