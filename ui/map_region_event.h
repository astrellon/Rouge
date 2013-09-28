#pragma once

#include <string>
using namespace std;

#include "event.h"

namespace am {
namespace game {
	class MapRegion;
	class GameObject;
}
using namespace am::game;

namespace ui {

	class MapRegionEvent : public Event {
	public:
		MapRegionEvent(const char *type, MapRegion *region, GameObject *obj);
		~MapRegionEvent();

		MapRegion *getMapRegion() const;
		GameObject *getGameObject() const;

	protected:

		MapRegion *mMapRegion;
		GameObject *mGameObject;

	};

}
}
