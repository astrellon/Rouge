#include "door.h"

#include "map.h"
#include "tile_instance.h"
#include "tile_type.h"
#include "engine.h"
#include "character.h"

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/utils.h>
using namespace am::util;

#include <math/math.h>

#include <sstream>

#include <log/logger.h>

#include <gl.h>
#include <gfx/gfx_texture.h>

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int Door::LUA_ID = LUA_ID_DOOR;
	const char *Door::LUA_TABLENAME = LUA_TABLE_DOOR;
	const char *Door::sLockTypeNames[] = 
	{
		"none", "unlocked", "locked", "locked_item", "locked_special", "max_lock_type"
	};

	Door::Door() :
		GameObject(),
		mGraphic(nullptr),
		mOpened(false),
		mLock(NONE)
	{
		mFixedToGrid = true;
		setName("Door");
		addEventListener(am::ui::Mouse::MOUSE_UP, this);

		mDoorRegion = new MapRegion(3, 3, 1);
		//mDoorRegion->addEventListener("region_entered", this);
		//mDoorRegion->addEventListener("region_exited", this);
		mDoorType = Engine::getEngine()->getTileType("door");
	}
	Door::Door(const Door &copy) :
		GameObject(copy),
		mOpened(copy.mOpened),
		mDoorType(copy.mDoorType),
		mDoorRegion(new MapRegion(*copy.mDoorRegion))
	{
		if (copy.mGraphic)
		{
			// Find our new graphic if there was one.
			for (int i = 0; i < getNumChildren(); i++)
			{
				Sprite *temp = dynamic_cast<Sprite *>(getChildAt(i));
				if (temp && temp->getAsset() == copy.mGraphic->getAsset())
				{
					mGraphic = temp;
				}
			}
		}
		//mDoorRegion->addEventListener("region_entered", this);
		//mDoorRegion->addEventListener("region_exited", this);
	}
	Door::~Door()
	{
		removeEventListener(am::ui::Mouse::MOUSE_UP, this);

		mDoorRegion->removeEventListener("region_entered", this);
		mDoorRegion->removeEventListener("region_exited", this);
	}

	void Door::onEvent(MapRegionEvent *e)
	{
		if (!e)
		{
			return;
		}

		// TODO Make this better
		if (e->getType().compare("region_entered") == 0)
		{
			setOpened(true);
		}
		else
		{
			setOpened(false);
		}
	}

	void Door::setGraphic(Sprite *graphic, bool calcCameraOffset)
	{
		am::base::Handle<Sprite> currentGraphic(mGraphic);
		if (mGraphic)
		{
			removeChild(mGraphic);
		}
		mGraphic = graphic;
		if (graphic)
		{
			graphic->setInteractive(true);
			addChild(mGraphic);
		}
		if (graphic && calcCameraOffset)
		{
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.5f;
		}
		updateGraphic();
	}
	Sprite *Door::getGraphic() const
	{
		return mGraphic;
	}

	void Door::setDoorType(TileType *type)
	{
		if (mDoorType.get() != type)
		{
			removeFromMap(mMap);
			mDoorType = type;
			addToMap(mMap);
		}
	}
	TileType *Door::getDoorType() const
	{
		return mDoorType;
	}

	MapRegion *Door::getDoorRegion() const
	{
		return mDoorRegion;
	}
	
	bool Door::interactWith(GameObject *interacter, bool byMovement)
	{
		if (isOpened() && !byMovement)
		{
			setOpened(false);
		}
		else if (interacter->getGameObjectType() == CHARACTER)
		{
			if (canOpenBy(dynamic_cast<Character *>(interacter)))
			{
				setOpened(true);
			}
			/*else
			{
				return true;
			}*/
		}
		else
		{
			setOpened(true);
		}
		return false;
	}

	void Door::update(float dt)
	{
		
	}
	bool Door::onGameTick(float dt)
	{
		return false;
	}
	
	void Door::receiveDamage(float damage)
	{
		
	}

	bool Door::canOpenBy(const GameObject *byObject)
	{
		if (mLock == NONE || mLock == UNLOCKED)
		{
			return true;
		}
		else if (mLock == LOCKED_ITEM)
		{
			// Look for key on character.
			return false;
		}
		else if (mLock == LOCKED_SPECIAL)
		{
			// Call Lua function, result should indicate if it can be opened.
			return false;
		}
		// Locked!
		return false;
	}
	void Door::setOpened(bool opened)
	{
		if (mOpened != opened)
		{
			mOpened = opened;
			updateGraphic();
			updateTileType();
		}
	}
	bool Door::isOpened() const
	{
		return mOpened;
	}

	void Door::setLock(LockType lock)
	{
		mLock = lock;
		updateTileType();
	}
	Door::LockType Door::getLock() const
	{
		return mLock;
	}

	void Door::setMap(Map *map)
	{
		removeFromMap(mMap);
		GameObject::setMap(map);
		addToMap(map);
		//updateTileType();
	}
	
	float Door::getWidth()
	{
		return mGraphic->getWidth();
	}
	float Door::getHeight()
	{
		return mGraphic->getHeight();
	}

	const char *Door::getGameObjectTypeName() const
	{
		return "door";
	}
	GameObject::GameObjectType Door::getGameObjectType() const
	{
		return DOOR;
	}

	data::IData *Door::serialise()
	{
		data::IData *obj_output = GameObject::serialise();
		data::Table *output = dynamic_cast<data::Table *>(obj_output);
		if (!output)
		{
			am_log("ERROR", "Save door from GameObject not a data::Table!");
			return nullptr;
		}

		output->at("opened", mOpened);

		if (mGraphic)
		{
			output->at("graphic", mGraphic->serialise());
		}
		output->at("lock", getLockTypeName(mLock));
		
		return output;
	}
	int Door::deserialise(LoadingState *state, data::IData *data)
	{
		int loadResult = GameObject::deserialise(state, data);
		if (loadResult < 1)
		{
			return loadResult;
		}

		am::base::Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{	// Shouldn't happen due to GameObject::deserialise
			return -1;
		}

		am::base::Handle<data::IData> tempData(dataMap->at("graphic"));
		if (tempData)
		{
			Sprite *graphic = new Sprite();
			graphic->deserialise(state, tempData);
			setGraphic(graphic, false);
		}

		am::base::Handle<data::String> str(dataMap->at<data::String>("lock"));
		if (str)
		{
			setLock(getLockType(str->string()));
		}

		return 1;
	}

	void Door::updateGraphic()
	{
		if (!mGraphic)
		{
			return;
		}

		if (mOpened)
		{
			mGraphic->setSubWindowFrame(1);
		}
		else
		{
			mGraphic->setSubWindowFrame(0);
		}
	}

	void Door::updateTileType()
	{
		if (!mMap)
		{
			return;
		}
		TileInstance *instance = mMap->getTileInstance(getGridLocationX(), getGridLocationY());
		if (!instance)
		{
			return;
		}
		if (mOpened || mLock == NONE || mLock == UNLOCKED)
		{
			instance->removeTileType(mDoorType);
		}
		else
		{
			instance->addTileType(mDoorType);
		}
	}

	void Door::removeFromMap(Map *map)
	{
		if (map)
		{
			if (mDoorType)
			{
				TileInstance *instance = map->getTileInstance(getGridLocationX(), getGridLocationY());
				if (instance)
				{
					instance->removeTileType(mDoorType);
				}
			}
			if (mDoorRegion)
			{
				map->removeMapRegion(mDoorRegion);
			}
		}
	}
	void Door::addToMap(Map *map)
	{
		if (map)
		{
			if (mDoorType)
			{
				TileInstance *instance = map->getTileInstance(getGridLocationX(), getGridLocationY());
				if (instance)
				{
					instance->addTileType(mDoorType);
				}
			}
			if (mDoorRegion)
			{
				mDoorRegion->setLocation(getGridLocationX() - 1, getGridLocationY() - 1);
				//map->addMapRegion(mDoorRegion);
			}
		}
	}

	const char *Door::getLockTypeName(LockType type)
	{
		if (type < 0 || type >= MAX_LOCK_TYPE)
		{
			return nullptr;
		}
		return sLockTypeNames[type];
	}
	const char *Door::getLockTypeName(int type)
	{
		if (type < 0 || type >= MAX_LOCK_TYPE)
		{
			return nullptr;
		}
		return sLockTypeNames[type];
	}
	Door::LockType Door::getLockType(const char *typeName)
	{
		if (!typeName || typeName[0] == '\0')
		{
			return MAX_LOCK_TYPE;
		}
		for (int i = 0; i < MAX_LOCK_TYPE; i++)
		{
			if (strcmp(typeName, sLockTypeNames[i]) == 0)
			{
				return static_cast<LockType>(i);
			}
		}
		return MAX_LOCK_TYPE;
	}
	Door::LockType Door::getLockType(int type)
	{
		if (type < 0 || type >= MAX_LOCK_TYPE)
		{
			return MAX_LOCK_TYPE;
		}
		return static_cast<LockType>(type);
	}

}
}
