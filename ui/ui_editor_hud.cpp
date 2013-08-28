#include "ui_editor_hud.h"

//#include <gfx/gfx_engine.h>

namespace am {
namespace ui {

	EditorHud::EditorHud()
	{
		setName("EditorHud");
		mSideSprite = new Sprite("editor:side_bar");
		addChild(mSideSprite);

		mMapWidth = new TextInput();
		addChild(mMapWidth);
		mMapWidth->setSize(40.0f, 16.0f);
		mMapWidth->setParentOffset(20.0f, 20.0f);
		mMapWidth->setMaxCharacters(4);
		mMapWidth->setRestriction(TextInput::INTEGER);

		mMapHeight = new TextInput(*mMapWidth);
		mMapHeight->setParentOffsetX(70.0f);
		addChild(mMapHeight);
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
