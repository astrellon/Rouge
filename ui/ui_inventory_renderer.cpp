#include "ui_inventory_renderer.h"

#include <gl.h>
#include <gfx/gfx_texture.h>

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
		mHitbox(new gfx::Renderable())
	{
		addChild(mHitbox);
		mHitbox->setInteractive(true);
		addEventListener(Mouse::MOUSE_UP, this);
	}
	InventoryRenderer::~InventoryRenderer()
	{
		removeEventListener(Mouse::MOUSE_UP, this);
	}

	float InventoryRenderer::getWidth()
	{
		if (mInventory != nullptr)
		{
			return mInventory->getSpacesX() * game::Inventory::getSpaceSizeX();
		}
		return 0.0f;
	}
	float InventoryRenderer::getHeight()
	{
		if (mInventory != nullptr)
		{
			return mInventory->getSpacesY() * game::Inventory::getSpaceSizeY();
		}
		return 0.0f;
	}

	void InventoryRenderer::onEvent( InventoryEvent *e )
	{
		if (e)
		{
			if (e->getInventoryEventType() == ui::Inventory::INVENTORY_ADD)
			{
				addItem(e->getItem(), e->getSpotX(), e->getSpotY());
			}
			else if (e->getInventoryEventType() == ui::Inventory::INVENTORY_REMOVE)
			{
				removeChild(e->getItem());
			}
		}
	}

	void InventoryRenderer::onEvent(MouseEvent *e)
	{
		if (e)
		{
			game::PlayerHand *hand = game::PlayerHand::getPlayerHand();

			float mouseX = 0.0f;
			float mouseY = 0.0f;
			mHitbox->getScreenToLocal(e->getMouseX(), e->getMouseY(), mouseX, mouseY);
			int gridX = math::round(mouseX / mInventory->getSpaceSizeX());
			int gridY = math::round(mouseY / mInventory->getSpaceSizeY());

			game::Item *item = mInventory->getItemAt(gridX, gridY);

			if (item != nullptr && hand->getInhand() == nullptr)
			{
				if (e->getMouseEventType() == Mouse::MOUSE_UP)
				{
					if (e->getMouseButton() == Mouse::LEFT_BUTTON)
					{
						if (hand->getInhand() == nullptr && mInventory->removeItem(item))
						{
							hand->setInhand(item);
							item->setItemLocation(Item::HAND);
						}
					}
					else if (e->getMouseButton() == Mouse::RIGHT_BUTTON)
					{
						Engine::getEngine()->getGameHud()->getInspector()->setInspectObject(item);
					}
				}
				e->stopPropagation();
			}
			else if (hand->getInhand() != nullptr)
			{
				if (mInventory->hasSpaceFor(hand->getInhand(), gridX, gridY) && mInventory->addItem(hand->getInhand(), gridX, gridY))
				{
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
		gfx::Texture::bindTexture(0);
		float width = mInventory->getSpacesX() * game::Inventory::getSpaceSizeX();
		float height = mInventory->getSpacesY() * game::Inventory::getSpaceSizeY();
		glBegin(GL_QUADS);
			glColor4f(0.7f, 0.3f, 0.1f, 0.35f);
			glVertex2f(0, 0);
			glVertex2f(width, 0);
			glVertex2f(width, height);
			glVertex2f(0, height);
		glEnd();
	}

	game::Inventory *InventoryRenderer::getInventory() const
	{
		return mInventory;
	}
	void InventoryRenderer::setInventory( game::Inventory *inventory )
	{
		if (inventory != mInventory)
		{
			if (mInventory)
			{
				mInventory->removeEventListener(ui::Inventory::EventTypeName[ui::Inventory::INVENTORY_ADD], this);
				mInventory->removeEventListener(ui::Inventory::EventTypeName[ui::Inventory::INVENTORY_REMOVE], this);
				const game::Inventory::InventorySpots &spots = mInventory->getInventory();
				game::Inventory::InventorySpots::const_iterator iter;
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
				mInventory->addEventListener(ui::Inventory::EventTypeName[ui::Inventory::INVENTORY_ADD], this);
				mInventory->addEventListener(ui::Inventory::EventTypeName[ui::Inventory::INVENTORY_REMOVE], this);
				const game::Inventory::InventorySpots &spots = mInventory->getInventory();
				game::Inventory::InventorySpots::const_iterator iter;
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

		item->setPosition(x * game::Inventory::getSpaceSizeX(), y * game::Inventory::getSpaceSizeY());
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
