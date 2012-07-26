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
		mInspector->setHeight(200.0f);

		mCharacterScreen = new CharacterScreen();
		addChild(mCharacterScreen);
		mCharacterScreen->setWidth(200.0f);
		mCharacterScreen->setHeight(300.0f);
	}
	GameHud::~GameHud()
	{
	}

	Inspector *GameHud::getInspector()
	{
		return mInspector;
	}
	CharacterScreen *GameHud::getCharacterScreen()
	{
		return mCharacterScreen;
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
