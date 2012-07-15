#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class EditorHud : public UIComponent {
	public:
		EditorHud();
		~EditorHud();

		Sprite *getSideSprite();
		void loadAssets();

		virtual void setHeight(float width);

	protected:

		Handle<Sprite> mSideSprite;

	};

}
}
