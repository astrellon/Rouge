#include "screen.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_sprite.h"
#include "gfx/gfx_asset.h"

#include "logger.h"

namespace am {
namespace base {

	Screen::Screen(const char *name) :
		mForeground(new Layer()),
		mBackground(new Layer()),
		mName(name)
	{
	}

	Screen::~Screen()
	{
		//am_log("TEST", "Screen destructor called");
	}

	Layer *Screen::getForeground()
	{
		return mForeground.get();
	}
	Layer *Screen::getBackground()
	{
		return mBackground.get();
	}

	string Screen::getName() const
	{
		return mName;
	}

	void Screen::loadDef(JsonValue loaded)
	{
		if (loaded.getType() != JV_OBJ)
		{
			stringstream ss;
			ss << "Unable to load screen '" << mName << "' from type '" << loaded.getTypeName() << '\'';
			am_log("SCR", ss.str().c_str());
			return;
		}

		if (loaded.has("background", JV_STR))
		{
			Handle<Sprite> background(new Sprite(loaded["background"].getCStr()));
			mBackground->addChild(background.get());
		}
		if (loaded.has("foreground", JV_STR))
		{
			Handle<Sprite> foreground(new Sprite(loaded["foreground"].getCStr()));
			mForeground->addChild(foreground.get());
		}
	}

}
}
