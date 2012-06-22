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

	ObjectList *Screen::getObjects()
	{
		return &mObjects;
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

	bool Screen::addGameObject(GameObject *object)
	{
		if (object == NULL)
		{
			return false;
		}
		ObjectList::const_iterator iter = findGameObject(object);
		if (iter == mObjects.end())
		{
			mObjects.push_back(object);
			object->setScreen(this);
			return true;
		}
		return false;
	}
	bool Screen::removeGameObject(GameObject *object)
	{
		if (object == NULL)
		{
			return false;
		}
		ObjectList::const_iterator iter = findGameObject(object);
		if (iter != mObjects.end())
		{
			mObjects.erase(iter);
			object->setScreen(NULL);
			return true;
		}
		return false;
	}
	bool Screen::hasGameObject(GameObject *object) const
	{
		if (object == NULL)
		{
			return false;
		}
		ObjectList::const_iterator iter = findGameObject(object);
		return iter != mObjects.end();
	}
	ObjectList::const_iterator Screen::findGameObject(GameObject *object) const
	{
		ObjectList::const_iterator iter;
		for (iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		{
			if (iter->get() == object)
			{
				break;
			}
		}
		return iter;
	}
}
}
