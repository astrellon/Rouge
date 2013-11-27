#pragma once

#include <base/handle.h>

#include <string>
#include <vector>

#include <ui/event_interface.h>
using namespace am::ui;

#include <math/vector.h>
using namespace am::math;

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class GameObject;

	class MapRegion : public EventInterface {
	public:

		typedef std::vector< base::Handle< MapRegion > > MapRegionList;

		MapRegion();
		MapRegion(int width, int height, int fillValue = 0);
		MapRegion(const MapRegion &copy);
		~MapRegion();

		virtual void setSize(int width, int height, int fillValue = 0);
		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual bool setData(int x, int y, int value);
		virtual bool getData(int x, int y, int &result) const;
		virtual int *getData() const;

		virtual void setLocation(const Vector2i &location);
		virtual void setLocation(int x, int y);
		virtual Vector2i getLocation() const;

		virtual bool intersectsWith(GameObject *obj);

		virtual void gameObjectEntered(GameObject *obj);
		virtual void gameObjectExited(GameObject *obj);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		int *mData;
		int mWidth;
		int mHeight;
		Vector2i mLocation;

	};

}
}
