#pragma once

#include <string>

#include "event.h"

namespace am {
namespace game {
	class MapRegion;
	class GameObject;
}
using namespace am::game;

namespace ui {

	class MapRegionEvent : public Event
	{
	public:
		MapRegionEvent(const char *type, game::MapRegion *region, game::GameObject *obj);
		~MapRegionEvent();

		game::MapRegion *getMapRegion() const;
		game::GameObject *getGameObject() const;

	protected:

		game::MapRegion *mMapRegion;
		game::GameObject *mGameObject;

	};

}
}
