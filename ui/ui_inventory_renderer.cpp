#include "ui_inventory_renderer.h"

#include <gl.h>

#include <game/inventory.h>
#include <game/inventory_spot.h>

#include <log/logger.h>

#include <ui/event.h>

#include <sstream>

namespace am {
namespace ui {


	InventoryRenderer::InventoryRenderer()
	{
		addEventListener(MOUSE_UP, this);
	}
	InventoryRenderer::~InventoryRenderer()
	{
		removeEventListener(MOUSE_UP, this);
	}

	float InventoryRenderer::getWidth()
	{
		if (mInventory != NULL)
		{
			return mInventory->getSpacesX() * Inventory::getSpaceSizeX();
		}
		return 0.0f;
	}
	float InventoryRenderer::getHeight()
	{
		if (mInventory != NULL)
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
			Item *item = dynamic_cast<Item *>(e->getEventTarget());
			if (item != NULL)
			{
				stringstream ss;
				ss << "Clicked on item: " << item->getFullItemName();
				am_log("ITEM", ss);
			}
		}
	}

	void InventoryRenderer::preRender(float dt)
	{
		UIComponent::preRender(dt);

		if (mInventory == NULL)
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
					//removeChild(iter->getItem());
					removeItem(iter->getItem());
				}
			}
			mInventory = inventory;
			if (mInventory)
			{
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
		if (item == NULL)
		{
			return;
		}

		item->setPosition(x * Inventory::getSpaceSizeX(), y * Inventory::getSpaceSizeY());
		addChild(item);
	}

	void InventoryRenderer::removeItem(Item *item)
	{
		if (item == NULL)
		{
			return;
		}

		removeChild(item);
	}
}
}
