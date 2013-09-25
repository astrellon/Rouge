#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
#include <vector>
using namespace std;

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

	class LoadingState;

	class MapRegion : public EventInterface {
	public:

		typedef vector< Handle< MapRegion > > MapRegionList;

		MapRegion();
		MapRegion(int width, int height);
		~MapRegion();

		virtual void setSize(int width, int height);
		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual void setData(int x, int y, int value);
		virtual int *getData() const;

		virtual void setLocation(const Vector2i &location);
		virtual void setLocation(int x, int y);
		virtual Vector2i getLocation() const;

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
