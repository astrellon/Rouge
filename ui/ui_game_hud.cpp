#include "ui_game_hud.h"

namespace am {
namespace ui {

	GameHud::GameHud()
	{
		mInspector = new Inspector();
		addChild(mInspector);
		mInspector->setParentAnchor(X_RIGHT, Y_TOP);
		mInspector->setAnchor(X_RIGHT, Y_TOP);
		mInspector->setWidth(140.0f);
		mInspector->setHeight(600.0f);
	}
	GameHud::~GameHud()
	{
	}

	Inspector *GameHud::getInspector()
	{
		return mInspector.get();
	}

	void GameHud::setWidth(float width)
	{
		UIComponent::setWidth(width);
	}
	void GameHud::setHeight(float height)
	{
		UIComponent::setHeight(height);
	}

}
}
