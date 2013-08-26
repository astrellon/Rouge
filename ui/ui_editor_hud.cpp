#include "ui_editor_hud.h"

//#include <gfx/gfx_engine.h>

namespace am {
namespace ui {

	EditorHud::EditorHud()
	{
		mSideSprite = new Sprite("editor:side_bar");
		mSideSprite->setHeight(600.0f);
		addChild(mSideSprite);
	}
	EditorHud::~EditorHud()
	{

	}

	void EditorHud::loadAssets()
	{

	}

	Sprite *EditorHud::getSideSprite()
	{
		return mSideSprite.get();
	}

	void EditorHud::setHeight(float height)
	{
		mSideSprite->setHeight(height);
		UIComponent::setHeight(height);
	}

}
}
