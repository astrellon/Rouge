#include "ui_inventory_renderer.h"

#include <gl.h>

#include <game/inventory.h>
#include <game/inventory_spot.h>
#include <game/player_hand.h>
#include <game/engine.h>

#include <log/logger.h>

#include <ui/event.h>
#include <ui/keyboard_manager.h>
#include <ui/ui_game_hud.h>
#include <ui/ui_inspector.h>

#include <math/math.h>

#include <sstream>

namespace am {
namespace ui {

	InventoryRenderer::InventoryRenderer() :
		mHitbox(new Renderable())
	{
		addChild(mHitbox);
		mHitbox->setInteractive(true);
		addEventListener(MOUSE_UP, this);
	}
	InventoryRenderer::~InventoryRenderer()
	{
		removeEventListener(MOUSE_UP, this);
	}

	float InventoryRenderer::getWidth()
	{
		if (mInventory != nullptr)
		{
			return mInventory->getSpacesX() * Inventory::getSpaceSizeX();
		}
		return 0.0f;
	}
	float InventoryRenderer::getHeight()
	{
		if (mInventory != nullptr)
		{
			return mInventory->getSpacesY() * Inventory::getSpaceSizeY();
		}
		return 0.0f;
	}

	void InventoryRenderer::onEvent( InventoryEvent *e )
	{
		if (e)
		{
			if (e->getInventoryEventType() == INVENTORY_ADD)
			{
				addItem(e->getItem(), e->getSpotX(), e->getSpotY());
			}
			else if (e->getInventoryEventType() == INVENTORY_REMOVE)
			{
				removeChild(e->getItem());
			}
		}
	}

	void InventoryRenderer::onEvent(MouseEvent *e)
	{
		if (e)
		{
			PlayerHand *hand = PlayerHand::getPlayerHand();

			float mouseX = 0.0f;
			float mouseY = 0.0f;
			mHitbox->getScreenToLocal(e->getMouseX(), e->getMouseY(), mouseX, mouseY);
			int gridX = math::round(mouseX / mInventory->getSpaceSizeX());
			int gridY = math::round(mouseY / mInventory->getSpaceSizeY());

			Item *item = mInventory->getItemAt(gridX, gridY);

			if (item != nullptr && hand->getInhand() == nullptr)
			{
				if (e->getMouseEventType() == MOUSE_UP)
				{
					if (e->getMouseButton() == LEFT_BUTTON)
					{
						if (hand->getInhand() == nullptr)
						{
							hand->setInhand(item);
						
							mInventory->removeItem(item);
							item->setItemLocation(Item::HAND);
						}
					}
					else if (e->getMouseButton() == RIGHT_BUTTON)
					{
						Engine::getEngine()->getGameHud()->getInspector()->setInspectObject(item);
					}
				}
				e->stopPropagation();
			}
			else if (hand->getInhand() != nullptr)
			{
				if (mInventory->hasSpaceFor(hand->getInhand(), gridX, gridY))
				{
					mInventory->addItem(hand->getInhand(), gridX, gridY);
					hand->setInhand(nullptr);
				}
				e->stopPropagation();
			}
		}
	}

	void InventoryRenderer::preRender(float dt)
	{
		UIComponent::preRender(dt);

		if (mInventory == nullptr)
		{
			return;
		}
		float width = mInventory->getSpacesX() * Inventory::getSpaceSizeX();
		float height = mInventory->getSpacesY() * Inventory::getSpaceSizeY();
		glBegin(GL_QUADS);
			glColor4f(0.7f, 0.3f, 0.1f, 0.35f);
			glVertex2f(0, 0);
			glVertex2f(width, 0);
			glVertex2f(width, height);
			glVertex2f(0, height);
		glEnd();
	}

	Inventory *InventoryRenderer::getInventory() const
	{
		return mInventory;
	}
	void InventoryRenderer::setInventory( Inventory *inventory )
	{
		if (inventory != mInventory)
		{
			if (mInventory)
			{
				mInventory->removeEventListener(InventoryEventTypeName[INVENTORY_ADD], this);
				mInventory->removeEventListener(InventoryEventTypeName[INVENTORY_REMOVE], this);
				const Inventory::InventorySpots &spots = mInventory->getInventory();
				Inventory::InventorySpots::const_iterator iter;
				for (iter = spots.begin(); iter != spots.end(); ++iter)
				{
					removeItem(iter->getItem());
				}
				mHitbox->setSize(0, 0);
			}
			mInventory = inventory;
			if (mInventory)
			{
				mHitbox->setSize(getWidth(), getHeight());
				mInventory->addEventListener(InventoryEventTypeName[INVENTORY_ADD], this);
				mInventory->addEventListener(InventoryEventTypeName[INVENTORY_REMOVE], this);
				const Inventory::InventorySpots &spots = mInventory->getInventory();
				Inventory::InventorySpots::const_iterator iter;
				for (iter = spots.begin(); iter != spots.end(); ++iter)
				{
					addItem(iter->getItem(), iter->getX(), iter->getY());
				}
			}
		}
	}

	void InventoryRenderer::addItem(Item *item, int x, int y)
	{
		if (item == nullptr)
		{
			return;
		}

		item->setPosition(x * Inventory::getSpaceSizeX(), y * Inventory::getSpaceSizeY());
		addChild(item);
	}

	void InventoryRenderer::removeItem(Item *item)
	{
		if (item == nullptr)
		{
			return;
		}

		removeChild(item);
	}
}
}
