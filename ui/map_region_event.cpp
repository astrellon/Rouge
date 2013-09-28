#include "map_region_event.h"

#include <game/game_object.h>
#include <game/map_region.h>
using namespace am::game;

namespace am {
namespace ui {

	MapRegionEvent::MapRegionEvent(const char *type, MapRegion *region, GameObject *obj) :
		Event(type),
		mMapRegion(region),
		mGameObject(obj)
	{
		if (region)
		{
			region->retain();
		}
		if (obj)
		{
			obj->retain();
		}
	}
	MapRegionEvent::~MapRegionEvent()
	{
		if (mMapRegion)
		{
			mMapRegion->release();
		}
		if (mGameObject)
		{
			mGameObject->release();
		}
	}

	MapRegion *MapRegionEvent::getMapRegion() const
	{
		return mMapRegion;
	}
	GameObject *MapRegionEvent::getGameObject() const
	{
		return mGameObject;
	}
	
}
}
