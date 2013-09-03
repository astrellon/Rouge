#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ui_component.h>
#include <ui/ui_text_input.h>
#include <ui/ui_text_button.h>

#include <game/game.h>

namespace am {
namespace ui {

	class EditorHud : public UIComponent, public IEventListener {
	public:
		EditorHud();
		~EditorHud();

		Sprite *getSideSprite();
		void loadAssets();

		virtual void setHeight(float width);

		virtual void onEvent(Event *e);
		virtual void onEvent(KeyboardEvent *e);
		virtual void onEvent(MouseEvent *e);

	protected:

		Handle<Sprite> mSideSprite;
		Handle<TextInput> mMapName;
		Handle<TextInput> mMapWidth;
		Handle<TextInput> mMapHeight;
		Handle<TextButton> mMakeMap;
		Handle<Game> mGame;
	};

}
}
