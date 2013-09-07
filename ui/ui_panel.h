#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class MouseEvent;

	class Panel : public IEventListener, public UIComponent {
	public:
		Panel();
		~Panel();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(MouseEvent *e);

		TextField *getTitleField();
		void setTitle(const char *title);
		const char *getTitle() const;

		Sprite *getBackSprite();

	protected:

		float mStartingWidth;
		float mStartingHeight;
		bool mResizing;
		Handle<TextField> mTitle;
		Handle<Sprite> mBack;
		float mDragX;
		float mDragY;

	};

}
}
