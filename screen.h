#pragma once

#include <string>
using namespace std;

#include "handle.h"
#include "imanaged.h"
#include "json_value.h"
using namespace am::util;

namespace am {

namespace gfx {
	class Layer;
	class Sprite;
	class GfxEngine;
}
using namespace am::gfx;

namespace base {

	class Screen : public IManaged {
	public:
		Screen(GfxEngine *engine, const char *name);
		~Screen();

		Layer *getBackground();
		Layer *getForeground();

		string getName() const;

		void loadDef(JsonValue loaded);

	protected:

		GfxEngine *mGfxEngine;

		Handle<Layer> mBackground;
		Handle<Layer> mForeground;

		string mName;
		// TODO: List of items and other things on this screen.
		// TODO: List of points that link to other screens.
	};

}
}
