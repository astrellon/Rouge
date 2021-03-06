#include "map_region.h"

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>

#include <game/game_object.h>
#include <game/engine.h>

#include <math/math.h>
#include <algorithm>

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int MapRegion::LUA_ID = LUA_ID_MAPREGION;
	const char *MapRegion::LUA_TABLENAME = LUA_TABLE_MAPREGION;

	MapRegion::MapRegion() :
		ui::EventInterface(),
		mData(nullptr),
		mWidth(0),
		mHeight(0)
	{
	}
	MapRegion::MapRegion(int width, int height, int fillValue) :
		ui::EventInterface(),
		mData(nullptr),
		mWidth(0),
		mHeight(0)
	{
		setSize(width, height, fillValue);
	}
	MapRegion::MapRegion(const MapRegion &copy) :
		mData(nullptr),
		mWidth(copy.mWidth),
		mHeight(copy.mHeight)
	{
		int total = mWidth * mHeight;
		mData = new int[total];
		for (int i = 0; i < total; i++)
		{
			mData[i] = copy.mData[i];
		}
	}
	MapRegion::~MapRegion()
	{
		if (mData)
		{
			delete [] mData;
			mData = nullptr;
		}
	}

	void MapRegion::setSize(int width, int height, int fillValue)
	{
		if (width != mWidth || height != mHeight)
		{
			int total = width * height;
			int *newData = new int[width * height];

			for (int i = 0; i < total; i++)
			{
				newData[i] = fillValue;
			}

			int maxWidth = math::minv(width, mWidth);
			int maxHeight = math::minv(height, mHeight);

			for (int y = 0; y < maxHeight; y++)
			{
				for (int x = 0; x < maxWidth; x++)
				{
					newData[y * width + x] = mData[y * mWidth + x];
				}
			}

			delete [] mData;
			mData = newData;
			mWidth = width;
			mHeight = height;
		}
	}
	int MapRegion::getWidth() const
	{
		return mWidth;
	}
	int MapRegion::getHeight() const
	{
		return mHeight;
	}

	bool MapRegion::setData(int x, int y, int value)
	{
		if (x < 0 || x >= mWidth || y < 0 || y > mHeight)
		{
			return false;
		}
		mData[y * mWidth + x] = value;
		return true;
	}
	bool MapRegion::getData(int x, int y, int &result) const
	{
		if (x < 0 || x >= mWidth || y < 0 || y > mHeight)
		{
			return false;
		}
		result = mData[y * mWidth + x];
		return true;
	}
	int *MapRegion::getData() const
	{
		return mData;
	}

	void MapRegion::setLocation(const math::Vector2i &location)
	{
		mLocation = location;
	}
	void MapRegion::setLocation(int x, int y)
	{
		mLocation.x = x;
		mLocation.y = y;
	}
	math::Vector2i MapRegion::getLocation() const
	{
		return mLocation;
	}

	void MapRegion::gameObjectEntered(GameObject *obj)
	{
		base::Handle<ui::MapRegionEvent> e(new ui::MapRegionEvent("region_entered", this, obj));
		fireEvent<ui::MapRegionEvent>(e);
		if (obj)
		{
			obj->fireEvent<ui::MapRegionEvent>(e);
		}
	}
	void MapRegion::gameObjectExited(GameObject *obj)
	{
		base::Handle<ui::MapRegionEvent> e(new ui::MapRegionEvent("region_exited", this, obj));
		fireEvent<ui::MapRegionEvent>(e);
		if (obj)
		{
			obj->fireEvent<ui::MapRegionEvent>(e);
		}
	}

	bool MapRegion::intersectsWith(GameObject *obj)
	{
		int minX = obj->getGridLocationX() - mLocation.x;
		int minY = obj->getGridLocationY() - mLocation.y;
		
		int maxX = math::maxv(0, math::minv(mWidth, minX + math::round(obj->getWidth() * Engine::getEngine()->getGridSizeResp())));
		int maxY = math::maxv(0, math::minv(mHeight, minY + math::round(obj->getHeight() * Engine::getEngine()->getGridSizeResp())));
		minX = math::maxv(0, math::minv(mWidth, minX));
		minY = max(0, math::minv(mHeight, minY));
		for (int y = minY; y < maxY; y++)
		{
			for (int x = minX; x < maxX; x++)
			{
				if (mData[y * mWidth + x] > 0)
				{
					return true;
				}
			}
		}
		return false;
	}

}
}
