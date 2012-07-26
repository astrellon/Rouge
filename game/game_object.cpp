#include "game_object.h"

#include "map.h"

#include "engine.h"
#include "tile_type.h"

namespace am {
namespace game {

	int GameObject::sGameIdCount = 1;
	GameObject::GameObjectIdMap GameObject::sGameObjects;

	GameObject::GameObject() :
		Layer(),
		mLocationX(0.0f),
		mLocationY(0.0f),
		mCameraOffsetX(0.0f),
		mCameraOffsetY(0.0f),
		mFixedToGrid(false),
		mOnlyOnPassable(false),
		mMap(NULL)
	{
		setName("GameObject");
		addGameObject(this);

	}
	GameObject::~GameObject()
	{
		removeGameObject(this);
	}

	void GameObject::setName(const char *name)
	{
		mName = name;
	}
	void GameObject::setName(const string &name)
	{
		mName = name;
	}
	string GameObject::getName() const
	{
		return mName;
	}

	void GameObject::setLocation(float x, float y, bool setDraw)
	{
		mLocationX = x;
		mLocationY = y;
		if (setDraw)
		{
			mTransform.setXY(x, y);
		}
		
	}
	float GameObject::getLocationX() const
	{
		return mLocationX;
	}
	float GameObject::getLocationY() const
	{
		return mLocationY;
	}

	void GameObject::setGridLocation(int x, int y, bool setDraw)
	{
		mLocationX = Engine::getEngine()->getGridXSize() * static_cast<float>(x);
		mLocationY = Engine::getEngine()->getGridYSize() * static_cast<float>(y);
		if (setDraw)
		{
			mTransform.setXY(mLocationX, mLocationY);
		}
	}
	int GameObject::getGridLocationX() const
	{
		return static_cast<int>(mLocationX * Engine::getEngine()->getGridXSizeResp());
	}
	int GameObject::getGridLocationY() const
	{
		return static_cast<int>(mLocationY * Engine::getEngine()->getGridYSizeResp());
	}

	void GameObject::move(float x, float y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridXSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridYSizeResp());
		int newGridX = static_cast<int>((mLocationX + x) * engine->getGridXSizeResp());
		int newGridY = static_cast<int>((mLocationY + y) * engine->getGridYSizeResp());
		float dx = 0.0f;
		float dy = 0.0f;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			dy = y;
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			dx = x;
		}
		setLocation(mLocationX + dx, mLocationY + dy);
	}

	void GameObject::moveGrid(int x, int y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridXSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridYSizeResp());
		int newGridX = gridX + x;
		int newGridY = gridY + y;
		float dx = 0.0f;
		float dy = 0.0f;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			dy = static_cast<float>(y) * engine->getGridYSize();
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			dx = static_cast<float>(x) * engine->getGridXSize();
		}
		if (mMap->isValidGridLocation(newGridX, newGridY, this))
		{
			dy = static_cast<float>(y) * engine->getGridYSize();
			dx = static_cast<float>(x) * engine->getGridXSize();
		}
		else
		{
			dy = 0.0f;
			dx = 0.0f;
		}
		setLocation(mLocationX + dx, mLocationY + dy);
	}

	void GameObject::setCameraOffset(float x, float y)
	{
		mCameraOffsetX = x;
		mCameraOffsetY = y;
	}
	float GameObject::getCameraOffsetX() const
	{
		return mCameraOffsetX;
	}
	float GameObject::getCameraOffsetY() const
	{
		return mCameraOffsetY;
	}

	void GameObject::setMap(Map *map)
	{
		if (mMap)
		{
			mMap->release();
		}
		mMap = map;
		if (mMap)
		{
			mMap->retain();
		}
	}
	Map *GameObject::getMap()
	{
		return mMap;
	}

	void GameObject::setFixedToGrid(bool fixed)
	{
		mFixedToGrid = fixed;
	}
	bool GameObject::isFixedToGrid() const
	{
		return mFixedToGrid;
	}

	void GameObject::setOnlyOnPassable(bool only)
	{
		mOnlyOnPassable = only;
	}
	bool GameObject::isOnlyOnPassable() const
	{
		return mOnlyOnPassable;
	}

	void GameObject::addPassibleType(TileType *tileType)
	{
		if (tileType != NULL)
		{
			mPassibleTypes.push_back(tileType);
		}
	}
	void GameObject::removePassibleType(TileType *tileType)
	{
		if (tileType)
		{
			return;
		}
		for (size_t i = 0; i < mPassibleTypes.size(); i++)
		{
			if (mPassibleTypes[i] == tileType)
			{
				mPassibleTypes.erase(mPassibleTypes.begin() + i);
			}
		}
	}
	void GameObject::removeAllPassibleTypes()
	{
		mPassibleTypes.clear();
	}
	bool GameObject::hasPassibleType(TileType *tileType) const
	{
		if (tileType == NULL)
		{
			return false;
		}
		for (size_t i = 0; i < mPassibleTypes.size(); i++)
		{
			if (mPassibleTypes[i] == tileType)
			{
				return true;
			}
		}
		return false;
	}
	GameObject::PassibleTypeList &GameObject::getPassibleTypes()
	{
		return mPassibleTypes;
	}
	const GameObject::PassibleTypeList &GameObject::getPassibleTypes() const
	{
		return mPassibleTypes;
	}

	void GameObject::setGameId(int id)
	{
		mGameId = id;
	}
	int GameObject::getGameId() const
	{
		return mGameId;
	}

	int GameObject::nextGameId()
	{
		return sGameIdCount++;
	}
	GameObject *GameObject::getByGameId(int id)
	{
		if (id == 0)
		{
			return NULL;
		}
		GameObjectIdMap::iterator iter = sGameObjects.find(id);
		if (iter == sGameObjects.end())
		{
			return iter->second;
		}
		return NULL;
	}

	void GameObject::addGameObject(GameObject *obj)
	{
		if (obj == NULL)
		{
			return;
		}
		int id = nextGameId();
		obj->setGameId(id);
		sGameObjects[id] = obj;
	}
	void GameObject::removeGameObject(GameObject *obj)
	{
		if (obj == NULL)
		{
			return;
		}
		GameObjectIdMap::iterator iter = sGameObjects.find(obj->getGameId());
		if (iter != sGameObjects.end())
		{
			sGameObjects.erase(iter);
		}
	}
}
}
