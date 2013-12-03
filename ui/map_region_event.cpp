#include "map_region_event.h"

#include <game/game_object.h>
#include <game/map_region.h>

namespace am {
namespace ui {

	MapRegionEvent::MapRegionEvent(const char *type, game::MapRegion *region, game::GameObject *obj) :
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

	game::MapRegion *MapRegionEvent::getMapRegion() const
	{
		return mMapRegion;
	}
	game::GameObject *MapRegionEvent::getGameObject() const
	{
		return mGameObject;
	}
	
}
}
