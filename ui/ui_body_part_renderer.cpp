#include "ui_body_part_renderer.h"

#include <gl.h>

#include <game/player_hand.h>
#include <game/inventory.h>
#include <game/character.h>
using namespace am::game;

#include <gfx/gfx_texture.h>

#include <log/logger.h>

#include <ui/event.h>
#include <ui/keyboard_manager.h>

#include <sstream>

namespace am {
namespace ui {

	BodyPartRenderer::BodyPartRenderer(short width, short height, const char *bodyPartName) :
		mBodyPartName(bodyPartName)
	{
		stringstream ss;
		ss << "BodyPartRenderer -> " << bodyPartName;
		setName(ss.str().c_str());

		setMaxItemSize(width, height);

		addEventListener(MOUSE_UP, this);
	}
	BodyPartRenderer::~BodyPartRenderer()
	{
		removeEventListener(MOUSE_UP, this);
	}

	void BodyPartRenderer::onEvent(EquipEvent *e)
	{
		if (e)
		{
			updateGraphic();
		}
	}

	void BodyPartRenderer::onEvent(MouseEvent *e)
	{
		if (e)
		{
			PlayerHand *hand = PlayerHand::getPlayerHand();

			Handle<Item> prevEquipped = mCurrentItem;
			if (mCharacter.get() != nullptr && mCurrentItem.get() != nullptr)
			{
				mCharacter->unequipItem(mBodyPartName.c_str());
			}

			if (hand->getInhand() != nullptr)
			{
				if (!mCharacter->equipItem(hand->getInhand(), mBodyPartName.c_str()))
				{
					return;
				}
			}

			hand->setInhand(prevEquipped);
			if (prevEquipped)
			{
				prevEquipped->setItemLocation(Item::HAND);
			}

			updateGraphic();

			e->stopPropagation();
		}
	}

	void BodyPartRenderer::setMaxItemSize(short width, short height)
	{
		mMaxItemWidth = width;
		mMaxItemHeight = height;

		setSize(static_cast<float>(width) * Inventory::getSpaceSizeX(), 
			static_cast<float>(height) * Inventory::getSpaceSizeY());
	}
	short BodyPartRenderer::getMaxItemWidth() const
	{
		return mMaxItemWidth;
	}
	short BodyPartRenderer::getMaxItemHeight() const
	{
		return mMaxItemHeight;
	}

	void BodyPartRenderer::setCharacter(Character *character)
	{
		if (mCharacter)
		{
			mCharacter->removeEventListener("equip", this);
			mCharacter->removeEventListener("unequip", this);
		}
		mCharacter = character;
		if (character)
		{
			mCharacter->addEventListener("equip", this);
			mCharacter->addEventListener("unequip", this);
		}
		updateGraphic();
	}
	Character *BodyPartRenderer::getCharacter() const
	{
		return mCharacter;
	}

	void BodyPartRenderer::setBodyPartName(const char *partName)
	{
		mBodyPartName = partName;
		updateGraphic();
	}
	const char *BodyPartRenderer::getBodyPartName() const
	{
		return mBodyPartName.c_str();
	}

	void BodyPartRenderer::preRender(float dt)
	{
		UIComponent::preRender(dt);

		float width = getWidth();
		float height = getHeight();

		gfx::Texture::bindTexture(0);
		glBegin(GL_QUADS);
			glColor4f(0.7f, 0.3f, 0.1f, 0.35f);
			glVertex2f(0, 0);
			glVertex2f(width, 0);
			glVertex2f(width, height);
			glVertex2f(0, height);
		glEnd();
	}

	bool BodyPartRenderer::interacteWithLayer() const
	{
		return true;
	}

	void BodyPartRenderer::updateGraphic()
	{
		if (mCharacter == nullptr || mBodyPartName.size() == 0)
		{
			if (mCurrentItem != nullptr)
			{
				removeChild(mCurrentItem);
			}
			return;
		}
		
		Handle<Item> equipped = mCharacter->getEquipped(mBodyPartName.c_str());
		if (equipped != nullptr && equipped != mCurrentItem)
		{
			addChild(equipped);
			float x = (getWidth() - equipped->getWidth()) * 0.5f;
			float y = (getHeight() - equipped->getHeight()) * 0.5f;
			equipped->setPosition(x, y);
		}
		mCurrentItem = equipped;
	}

}
}
