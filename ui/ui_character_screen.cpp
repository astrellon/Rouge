#include "ui_character_screen.h"

#include <sstream>

#include <game/stats_common.h>
#include <game/stats.h>

namespace am {
namespace ui {

	CharacterScreen::CharacterScreen() :
		Panel(),
		mTextDirty(true)
	{
		mInfo = new TextField();
		mInfo->setColour(0, 0, 0);
		mInfo->setPosition(10, 22);
		mInfo->setInteractive(true);
		addChild(mInfo);

		setTitle("Character Screen");
	}
	CharacterScreen::~CharacterScreen()
	{
	}

	void CharacterScreen::setCharacter(Character *character)
	{
		if (character != mCharacter)
		{
			mCharacter = character;
			mTextDirty = true;
		}
	}
	Character *CharacterScreen::getCharacter()
	{
		return mCharacter;
	}

	void CharacterScreen::setWidth(float width)
	{
		Panel::setWidth(width);
		mInfo->setWidth(mWidth - 18);
	}
	void CharacterScreen::setHeight(float height)
	{
		Panel::setHeight(height);
		mInfo->setHeight(mHeight - 28);
	}

	TextField *CharacterScreen::getTextField()
	{
		return mInfo;
	}

	void CharacterScreen::setTextDirty()
	{
		mTextDirty = true;
	}

	void CharacterScreen::render(float dt)
	{
		if (mTextDirty)
		{
			updateText();
		}
		UIComponent::render(dt);
	}

	void CharacterScreen::updateText()
	{
		if (mCharacter.get() == NULL)
		{
			mInfo->setText("No character selected");
			return;
		}

		stringstream ss;
		ss << "<colour yellow>Name:</colour> " << mCharacter->getName() << "\n";
		ss << "-------\n";

		Stats &stats = mCharacter->getStats();
		ss << "<colour 0.8 0.15 0.1>Health:</colour> " << stats.getStat(Stat::HEALTH) << " / " << stats.getStat(Stat::MAX_HEALTH) << "\n";
		ss << "<colour white>Strength:</colour> " << stats.getStat(Stat::STRENGTH) << "\n";
		ss << "<colour white>Dexderity:</colour> " << stats.getStat(Stat::DEXDERITY) << "\n";

		mInfo->setText(ss.str());

		mTextDirty = false;
	}

}
}
