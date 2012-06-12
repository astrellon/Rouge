#include "screen.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_layer.h"

namespace am {
namespace base {

	Screen::Screen(GfxEngine *engine, const char *name) :
		mForeground(new Layer(engine)),
		mBackground(new Layer(engine)),
		mName(name)
	{
	}

	Screen::~Screen()
	{
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

}
}
