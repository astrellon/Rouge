#include "map_region.h"

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
using namespace am::util;

#include <game/game_object.h>
#include <game/engine.h>

#include <math/math.h>

namespace am {
namespace game {

	const int MapRegion::LUA_ID = 0x27;
	const char *MapRegion::LUA_TABLENAME = "am_game_MapRegion";

	MapRegion::MapRegion() :
		EventInterface(),
		mData(nullptr),
		mWidth(0),
		mHeight(0)
	{
	}
	MapRegion::MapRegion(int width, int height) :
		EventInterface(),
		mData(nullptr),
		mWidth(0),
		mHeight(0)
	{
		setSize(width, height);
	}
	MapRegion::~MapRegion()
	{
		if (mData)
		{
			delete [] mData;
			mData = nullptr;
		}
	}

	void MapRegion::setSize(int width, int height)
	{
		if (width != mWidth || height != mHeight)
		{
			int total = width * height;
			int *newData = new int[width * height];

			for (int i = 0; i < total; i++)
			{
				newData[i] = 0;
			}

			int maxWidth = min(width, mWidth);
			int maxHeight = min(height, mHeight);

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

	void MapRegion::setData(int x, int y, int value)
	{
		if (x < 0 || x >= mWidth || y < 0 || y > mHeight)
		{
			return;
		}
		mData[y * mWidth + x] = value;
	}
	int *MapRegion::getData() const
	{
		return mData;
	}

	void MapRegion::setLocation(const Vector2i &location)
	{
		mLocation = location;
	}
	void MapRegion::setLocation(int x, int y)
	{
		mLocation.x = x;
		mLocation.y = y;
	}
	Vector2i MapRegion::getLocation() const
	{
		return mLocation;
	}

	void MapRegion::gameObjectEntered(GameObject *obj)
	{
		Handle<MapRegionEvent> e(new MapRegionEvent("region_entered", this, obj));
		fireEvent<MapRegionEvent>(e);
		if (obj)
		{
			obj->fireEvent<MapRegionEvent>(e);
		}
	}
	void MapRegion::gameObjectExited(GameObject *obj)
	{
		Handle<MapRegionEvent> e(new MapRegionEvent("region_exited", this, obj));
		fireEvent<MapRegionEvent>(e);
		if (obj)
		{
			obj->fireEvent<MapRegionEvent>(e);
		}
	}

	bool MapRegion::interspectsWith(GameObject *obj)
	{
		int minX = max(0, obj->getGridLocationX() - mLocation.x);
		int minY = max(0, obj->getGridLocationY() - mLocation.y);
		int maxX = min(mWidth, minX + round(obj->getWidth() * Engine::getEngine()->getGridSizeResp()));
		int maxY = min(mHeight, minY + round(obj->getHeight() * Engine::getEngine()->getGridSizeResp()));

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
