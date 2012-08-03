#include "ui_inventory_renderer.h"

#include <game/inventory.h>
#include <game/inventory_spot.h>

#include <log/logger.h>

#include <ui/event.h>

#include <sstream>

namespace am {
namespace ui {


	InventoryRenderer::InventoryRenderer()
	{

	}
	InventoryRenderer::~InventoryRenderer()
	{

	}

	void InventoryRenderer::setWidth( float width )
	{
		UIComponent::setWidth(width);
	}
	void InventoryRenderer::setHeight( float height )
	{
		UIComponent::setHeight(height);
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

	void InventoryRenderer::render(float dt)
	{
		UIComponent::render(dt);
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
					removeChild(iter->getItem());
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

}
}
