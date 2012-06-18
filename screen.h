#pragma once

#include <string>
#include <vector>
using namespace std;

#include "handle.h"
#include "imanaged.h"
#include "json_value.h"
using namespace am::util;

#include "game_object.h"

namespace am {

namespace gfx {
	class Layer;
	class Sprite;
}
using namespace am::gfx;

namespace base {

	class Screen : public IManaged {
	public:

		Screen(const char *name);
		~Screen();

		Layer *getBackground();
		Layer *getForeground();

		string getName() const;

		void loadDef(JsonValue loaded);

		ObjectList *getObjects();

	protected:

		Handle<Layer> mBackground;
		Handle<Layer> mForeground;

		ObjectList mObjects;

		string mName;
		// TODO: List of items and other things on this screen.
		// TODO: List of points that link to other screens.
	};

}
}
