#include "ui_body_part_renderer.h"

#include <gl.h>

#include <game/player_hand.h>
#include <game/inventory.h>
using namespace am::game;

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
	}
	BodyPartRenderer::~BodyPartRenderer()
	{

	}

	void BodyPartRenderer::onEvent(MouseEvent *e)
	{
		if (e)
		{
			/*PlayerHand *hand = PlayerHand::getPlayerHand();

			int gridX = static_cast<int>(static_cast<float>(e->getLocalMouseX()) / Inventory::getSpaceSizeX());
			int gridY = static_cast<int>(static_cast<float>(e->getLocalMouseY()) / Inventory::getSpaceSizeY());

			Item *item = mInventory->getItemAt(gridX, gridY);

			if (item != NULL && hand->getInhand() == NULL)
			{
				if (e->getMouseEventType() == MOUSE_UP)
				{
					if (hand->getInhand() == NULL)
					{
						item->setInteractive(false);
						hand->setInhand(item);
						
						mInventory->removeItem(item);
						item->setItemLocation(Item::HAND);
					}
				}
				e->stopPropagation();
			}
			else if (hand->getInhand() != NULL)
			{
				if (mInventory->hasSpaceFor(hand->getInhand(), gridX, gridY))
				{
					mInventory->addItem(hand->getInhand(), gridX, gridY);
					hand->setInhand(NULL);
				}
				e->stopPropagation();
			}*/
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
		mCharacter = character;
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
		if (mCharacter == NULL || mBodyPartName.size() == 0)
		{
			if (mCurrentItem.get() != NULL)
			{
				removeChild(mCurrentItem);
			}
			return;
		}
		
		Handle<Item> equipped = mCharacter->getEquipped(mBodyPartName.c_str());
		if (equipped.get() != NULL && equipped.get() != mCurrentItem.get())
		{
			addChild(equipped);
			float x = (getWidth() - equipped->getWidth()) * 0.5f;
			float y = (getHeight() - equipped->getHeight()) * 0.5f;
			equipped->setPosition(x, y);

			mCurrentItem = equipped;
		}
	}

}
}
