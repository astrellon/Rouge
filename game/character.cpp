#include "character.h"

#include "engine.h"
#include "map.h"

#include <sstream>
using namespace std;

#include <log/logger.h>

namespace am {
namespace game {

	Character::Character() :
		GameObject(),
		mGraphic(NULL),
		mController(NULL),
		mMoveX(0),
		mMoveY(0)
	{
		setName("Character");
		mPickupReach = Engine::getEngine()->getGridXSize() * 1.5f;

		addEventListener(MOUSE_UP, this);

		mInventory = new Inventory(10, 6);
	}
	Character::~Character()
	{

	}

	void Character::setGraphic(Sprite *graphic)
	{
		if (mGraphic.get())
		{
			removeChild(mGraphic.get());
		}
		mGraphic = graphic;
		if (graphic)
		{
			addChild(graphic);
			// Aim for head-ish area.
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.3f;
		}
	}
	Sprite *Character::getGraphic()
	{
		return mGraphic.get();
	}

	void Character::update(float dt)
	{
		if (mController.get())
		{
			mController->update(this, dt);
		}
		if (mMoveX != 0 || mMoveY != 0)
		{
			Engine *engine = Engine::getEngine();
			moveGrid(mMoveX, mMoveY);
			//float dx = engine->getGridXSize() * static_cast<float>(mMoveX);
			//float dy = engine->getGridYSize() * static_cast<float>(mMoveY);

			//move(dx, dy);
		}
	}

	void Character::setController(IController *controller)
	{
		mController = controller;
	}
	IController *Character::getController()
	{
		return mController.get();
	}
	/*
	void Character::move(float x, float y)
	{
		//int gridX = static_cast<int>((mLocationX + x) * Engine::getEngine()->getGridXSizeResp());
		//int gridY = static_cast<int>((mLocationY + y) * Engine::getEngine()->getGridXSizeResp());
		setLocation(mLocationX + x, mLocationY + y);
	}
	*/
	void Character::setMoveVector(int x, int y)
	{
		mMoveX = x;
		mMoveY = y;
	}
	int Character::getMoveVectorX() const
	{
		return mMoveX;
	}
	int Character::getMoveVectorY() const
	{
		return mMoveY;
	}

	void Character::setPickupReach(float reach)
	{
		if (reach < 0.0f) {
			reach = -reach;
		}
		mPickupReach = reach;
	}
	float Character::getPickupReach() const
	{
		return mPickupReach;
	}

	float Character::getWidth()
	{
		return mGraphic->getWidth();
	}
	float Character::getHeight()
	{
		return mGraphic->getHeight();
	}

	Stats &Character::getStats()
	{
		return mStats;
	}
	
	Inventory *Character::getInventory()
	{
		return mInventory;
	}

	bool Character::pickupItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}
		if (item->getItemLocation() == Item::GROUND)
		{
			float dx = item->getLocationX() - getLocationX();
			float dy = item->getLocationY() - getLocationY();
			if (dx > mPickupReach || dx < -mPickupReach || dy > mPickupReach || dy < -mPickupReach)
			{
				// Too far away.
				return false;
			}
		}
		// There may not be space so this can still return false.
		return addItem(item);
	}
	bool Character::addItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->addItem(item);
	}
	bool Character::removeItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->removeItem(item);
	}
	bool Character::hasItem(const Item *item) const
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->hasItem(item);
	}
	bool Character::dropItem(Item *item, float x, float y)
	{
		if (item == NULL || mMap == NULL ||
			x < 0 || y < 0 ||
			x >= mMap->getWidth() ||
			y >= mMap->getHeight())
		{
			am_log("CHAR", "Off map");
			return false;
		}

		/*int dx = getGridLocationX() - gridX;
		int dy = getGridLocationY() - gridY;
		if (dx < -1 || dx > 1 || dy < -1 || dy > 1)
		{
			am_log("CHAR", "Too far away");
			return false;
		}*/
		float dx = getLocationX() - x;
		float dy = getLocationY() - y;
		if (dx < -mPickupReach || dx > mPickupReach || dy < -mPickupReach || dy > mPickupReach)
		{
			am_log("CHAR", "Too far away");
			return false;
		}

		//if (!mMap->isValidGridLocation(gridX, gridY, item))
		if (!mMap->isValidLocation(x, y, item))
		{
			am_log("CHAR", "Invalid location for item");
			return false;
		}
		//item->setGridLocation(gridX, gridY);
		item->setLocation(x, y);
		mMap->addGameObject(item);
		item->setItemLocation(Item::GROUND);

		mInventory->removeItem(item);
		return true;
	}

	const char *Character::getGameObjectTypeName() const
	{
		return "character";
	}

}
}
