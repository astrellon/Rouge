#include "ui_game_hud.h"

namespace am {
namespace ui {

	GameHud::GameHud() :
		UIComponent()
	{
		mInspector = new Inspector();
        mInspector->show();
		addChild(mInspector);
		mInspector->setParentAnchor(X_RIGHT, Y_TOP);
		mInspector->setAnchor(X_RIGHT, Y_TOP);
		mInspector->setSize(140.0f, 200.0f);

		mCharacterScreen = new CharacterScreen();
        mCharacterScreen->show();
		addChild(mCharacterScreen);
		mCharacterScreen->setSize(200.0f, 300.0f);

		mDialogueBox = new DialogueBox();
		addChild(mDialogueBox);
		mDialogueBox->setParentAnchor(X_LEFT, Y_BOTTOM);
		mDialogueBox->setAnchor(X_LEFT, Y_BOTTOM);
		mDialogueBox->setSize(200.0f, 100.0f);
		mDialogueBox->setParentOffset(20.0f, -20.0f);
		
		mDialogueChoices = new DialogueChoices();
		addChild(mDialogueChoices);
		mDialogueChoices->setParentAnchor(X_RIGHT, Y_BOTTOM);
		mDialogueChoices->setAnchor(X_RIGHT, Y_BOTTOM);
		mDialogueChoices->setSize(200.0f, 100.0f);
		mDialogueChoices->setParentOffset(-20.0f, -20.0f);

		mStoreScreen = new StoreScreen();
		addChild(mStoreScreen);

	}
	GameHud::~GameHud()
	{
	}

	Inspector *GameHud::getInspector() const
	{
		return mInspector;
	}
	CharacterScreen *GameHud::getCharacterScreen() const
	{
		return mCharacterScreen;
	}
	DialogueBox *GameHud::getDialogueBox() const
	{
		return mDialogueBox;
	}
	DialogueChoices *GameHud::getDialogueChoices() const
	{
		return mDialogueChoices;
	}
	StoreScreen *GameHud::getStoreScreen() const
	{
		return mStoreScreen;
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
