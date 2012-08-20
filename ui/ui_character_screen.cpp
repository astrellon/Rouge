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
		setName("CharacterScreen");
		mInfo = new TextField2();
		mInfo->setColour(0, 0, 0);
		mInfo->setPosition(10, 22);
		mInfo->setInteractive(true);
		addChild(mInfo);

		mInventory = new InventoryRenderer();
		mInventory->setName("InventoryRenderer");
		addChild(mInventory);
		mInventory->setOffset(-16.0f, 16.0f);
		mInventory->setAnchorY(Y_BOTTOM);
		mInventory->setParentAnchorY(Y_BOTTOM);

		setInteractive(true);
		mInventory->setInteractive(true);

		setTitle("Character Screen");
	}
	CharacterScreen::~CharacterScreen()
	{
	}

	void CharacterScreen::onEvent(EquipEvent *e)
	{
		if (e)
		{
			mTextDirty = true;
		}
	}

	void CharacterScreen::setCharacter(Character *character)
	{
		if (character != mCharacter)
		{
			if (mCharacter.get() != NULL)
			{
				mCharacter->removeEventListener("equip", this);
				mCharacter->removeEventListener("unequip", this);
			}
			mCharacter = character;
			mTextDirty = true;
			if (mCharacter)
			{
				mInventory->setInventory(character->getInventory());
				mCharacter->addEventListener("equip", this);
				mCharacter->addEventListener("unequip", this);
			}
			else
			{
				mInventory->setInventory(NULL);
			}
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

	TextField2 *CharacterScreen::getTextField()
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
		ss << "<char_screen>";
		ss << "<title class='name'>Name:</title> " << mCharacter->getName() << "\n";
		ss << "-------\n";

		Stats &stats = mCharacter->getStats();
		ss << "<title class='health'>Health:</title> " << stats.getStat(Stat::HEALTH) << " / " << stats.getStat(Stat::MAX_HEALTH) << "\n";
		ss << "<title class='armour'>Armour:</title> " << stats.getStat(Stat::ARMOUR) << "\n";
		ss << "<title class='strength'>Strength:</title> " << stats.getStat(Stat::STRENGTH) << "\n";
		ss << "<title class='dexterity'>Dexterity:</title> " << stats.getStat(Stat::DEXTERITY) << "\n";
		ss << "<title class='constitution'>Constitution:</title> " << stats.getStat(Stat::CONSTITUTION) << "\n";
		ss << "<title class='divine'>Divine:</title> " << stats.getStat(Stat::DIVINE) << "\n";
		ss << "<title class='arcane'>Arcane:</title> " << stats.getStat(Stat::ARCANCE) << "\n";
		ss << "<title class='damage'>Damage:</title> " << stats.getStat(Stat::MIN_DAMAGE) << " - " << stats.getStat(Stat::MAX_DAMAGE) << "\n";
		
		ss << "</char_screen>";
		mInfo->setText(ss.str());

		mTextDirty = false;
	}

}
}
