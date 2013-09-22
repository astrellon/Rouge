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

		mLeftHand = new BodyPartRenderer(2, 4, "left_hand");
		mLeftHand->setParentOffset(16, 172);
		addChild(mLeftHand);

		mRightHand = new BodyPartRenderer(2, 4, "right_hand");
		mRightHand->setParentOffset(104, 172);
		addChild(mRightHand);

		mHead = new BodyPartRenderer(2, 2, "head");
		mHead->setParentOffset(60, 150);
		addChild(mHead);

		mTorso = new BodyPartRenderer(2, 2, "torso");
		mTorso->setParentOffset(60, 190);
		addChild(mTorso);

		mLegs = new BodyPartRenderer(2, 2, "legs");
		mLegs->setParentOffset(60, 230);
		addChild(mLegs);

		setTitle("Character Screen");
	}
	CharacterScreen::~CharacterScreen()
	{
		setCharacter(nullptr);
	}

	void CharacterScreen::onEvent(Event *e)
	{
		if (e)
		{
			mTextDirty = true;
		}
	}
	void CharacterScreen::onEvent(EquipEvent *e)
	{
		if (e)
		{
			mTextDirty = true;
		}
	}
	void CharacterScreen::onEvent(StatEvent *e)
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
			if (mCharacter.get() != nullptr)
			{
				mCharacter->removeEventListener("equip", this);
				mCharacter->removeEventListener("unequip", this);
				mCharacter->removeEventListener("stat_change", this);
				mCharacter->removeEventListener("level_change", this);
				mCharacter->removeEventListener("experience_change", this);
			}
			mCharacter = character;
			mTextDirty = true;
			if (mCharacter)
			{
				mInventory->setInventory(character->getInventory());
				mCharacter->addEventListener("equip", this);
				mCharacter->addEventListener("unequip", this);
				mCharacter->addEventListener("stat_change", this);
				mCharacter->addEventListener("level_change", this);
				mCharacter->addEventListener("experience_change", this);
			}
			else
			{
				mInventory->setInventory(nullptr);
			}
			mLeftHand->setCharacter(character);
			mRightHand->setCharacter(character);
			mTorso->setCharacter(character);
			mHead->setCharacter(character);
			mLegs->setCharacter(character);
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
		if (mCharacter.get() == nullptr)
		{
			mInfo->setText("No character selected");
			return;
		}

		stringstream ss;
		ss << "<char_screen>";
		ss << "<title class='name'>Name:</title> " << mCharacter->getName() << "\n";
		ss << "-------\n";
		ss << "<title class='level'>Level:</title> " << mCharacter->getLevel() << " (" << mCharacter->getExperience() << ")\n";

		Stats *stats = mCharacter->getStats();
		ss << "<title class='health'>Health:</title> " << stats->getStat(Stat::HEALTH) << " / " << stats->getStat(Stat::MAX_HEALTH) << "\n";
		ss << "<title class='armour'>Armour:</title> " << stats->getStat(Stat::ARMOUR) << "\n";
		ss << "<title class='strength'>Strength:</title> " << stats->getStat(Stat::STRENGTH) << "\n";
		ss << "<title class='dexterity'>Dexterity:</title> " << stats->getStat(Stat::DEXTERITY) << "\n";
		ss << "<title class='constitution'>Constitution:</title> " << stats->getStat(Stat::CONSTITUTION) << "\n";
		ss << "<title class='divine'>Divine:</title> " << stats->getStat(Stat::DIVINE) << "\n";
		ss << "<title class='arcane'>Arcane:</title> " << stats->getStat(Stat::ARCANE) << "\n";
		ss << "<title class='speed'>Speed:</title> " << stats->getStat(Stat::SPEED) << "\n";
		ss << "<title class='damage'>Damage:</title> " << stats->getStat(Stat::MIN_DAMAGE) << " - " << stats->getStat(Stat::MAX_DAMAGE) << "\n";
		
		ss << "</char_screen>";
		mInfo->setText(ss.str());

		mTextDirty = false;
	}

}
}
