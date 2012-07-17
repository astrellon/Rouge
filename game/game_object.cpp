#include "game_object.h"

#include "map.h"

#include "engine.h"
#include "tile_type.h"

namespace am {
namespace game {

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
	}
	GameObject::~GameObject()
	{

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
		return static_cast<int>(mLocationX / Engine::getEngine()->getGridXSize());
	}
	int GameObject::getGridLocationY() const
	{
		return static_cast<int>(mLocationY / Engine::getEngine()->getGridYSize());
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
		for (int i = 0; i < mPassibleTypes.size(); i++)
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
		for (int i = 0; i < mPassibleTypes.size(); i++)
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
		//return mPassibility;
	}

}
}
