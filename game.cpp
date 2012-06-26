#ifndef _ANDROID
#	include "game.h"
#endif

#include "engine.h"
#include "json_value.h"

#include "logger.h"
#include <sstream>
#include <stdio.h>

#include "gfx/gfx_layer.h"
#include "gfx/gfx_engine.h"
#include "screen.h"

using namespace am::util;

namespace am {
namespace base {

	Game::Game(Engine *engine) :
		//mMapFilename(""),
		mEngine(engine),
		mCurrentScreen(NULL)
	{
		mGameLayer = new Layer();
		mGameLayer->setName("Game->GameLayer");
		//GfxEngine::getEngine()->getRootLayer()->addChild(mGameLayer.get());

		mBackground = new Layer();
		mBackground->setName("Background");
		mGameLayer->addChild(mBackground.get());

		mItemLayer = new Layer();
		mItemLayer->setName("ItemLayer");
		mGameLayer->addChild(mItemLayer.get());

		mCharacterLayer = new Layer();
		mCharacterLayer->setName("CharacterLayer");
		mGameLayer->addChild(mCharacterLayer.get());

		mForeground = new Layer();
		mForeground->setName("Foreground");
		mGameLayer->addChild(mForeground.get());
	}
	Game::~Game()
	{
	}

	Screen *Game::getScreen(const char *screenName)
	{
		return getScreen(string(screenName));
	}
	Screen *Game::getScreen(const string &screenName)
	{
		ScreenMap::iterator iter = mScreens.find(screenName);
		if (iter != mScreens.end())
		{
			return iter->second.get();
		}

		stringstream ss;
		ss << "data/screens/" << screenName << "/screen.ssff";
		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load screen '" << screenName << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("SCREEN", errss);
			return NULL;
		}

		Handle<Screen> screen(new Screen(screenName.c_str()));
		screen->loadDef(loaded);

		mScreens[screenName] = screen;

		return screen.get();
	}

	Screen *Game::getCurrentScreen()
	{
		return mCurrentScreen.get();
	}
	
	void Game::setCurrentScreen(Screen *screen)
	{
		mBackground->clear();
		mItemLayer->clear();
		mCharacterLayer->clear();
		mForeground->clear();
		mCamera.followObject(NULL);

		mCurrentScreen = screen;
		if (screen)
		{
			mBackground->addChild(screen->getBackground());
			mForeground->addChild(screen->getForeground());
			mActiveObjects = screen->getObjects();

			if (mActiveObjects)
			{
				ObjectList::iterator iter;
				for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
				{
					mCharacterLayer->addChild(iter->get());
				}
			}
			else
			{
				stringstream errss;
				errss << "Screen (" << screen->getName() << ") return a NULL object list.";
				am_log("SCR", errss);
			}
		}
	}
	void Game::setCurrentScreen(const char *screenName)
	{
		setCurrentScreen(getScreen(string(screenName)));
	}
	void Game::setCurrentScreen(const string &screenName)
	{
		setCurrentScreen(getScreen(screenName));
	}

	bool Game::addGameObject(GameObject *object)
	{
		if (mCurrentScreen.get() == NULL)
		{
			am_log("SCR", "Unable to add game object to null current screen");
			return false;
		}
		if (mCurrentScreen->addGameObject(object))
		{
			mCharacterLayer->addChild(object);
			return true;
		}
		return false;
	}
	bool Game::removeGameObject(GameObject *object)
	{
		if (mCurrentScreen.get() == NULL)
		{
			am_log("SCR", "Unable to remove game object from null current screen");
			return false;
		}
		if (mCurrentScreen->removeGameObject(object))
		{
			mCharacterLayer->removeChild(object);
			return true;
		}
		return false;
	}
	bool Game::hasGameObject(GameObject *object) const
	{
		if (mCurrentScreen.get() == NULL)
		{
			am_log("SCR", "Unable to search for game object with null current screen");
			return false;
		}
		return mCurrentScreen->hasGameObject(object);
	}

	void Game::moveObjectToScreen(GameObject *object, const char *screenName, float x, float y, bool setAsCurrent)
	{
		if (!object)
		{
			return;
		}
		moveObjectToScreen(object, getScreen(screenName), x, y, setAsCurrent);
	}
	void Game::moveObjectToScreen(GameObject *object, Screen *screen, float x, float y, bool setAsCurrent)
	{
		if (!object || !screen)
		{
			return;
		}
		Screen *currentScreen = object->getScreen();
		if (currentScreen)
		{
			if (currentScreen->removeGameObject(object) && currentScreen == mCurrentScreen.get())
			{
				mCharacterLayer->removeChild(object);
			}
		}
		bool following = false;
		if (setAsCurrent)
		{
			following = mCamera.getFollowing() == object;
			setCurrentScreen(screen);
		}
		object->setScreen(screen);
		object->setLocation(x, y);
		if (following)
		{
			mCamera.followObject(object);
		}
		if (screen)
		{
			if (screen->addGameObject(object) && screen == mCurrentScreen.get())
			{
				mCharacterLayer->addChild(object);
			}
		}
	}

	Camera *Game::getCamera()
	{
		return &mCamera;
	}

	Layer *Game::getGameLayer()
	{
		return mGameLayer.get();
	}
	Layer *Game::getBackground()
	{
		return mBackground.get();
	}
	Layer *Game::getItemLayer()
	{
		return mItemLayer.get();
	}
	Layer *Game::getCharacterLayer()
	{
		return mCharacterLayer.get();
	}
	Layer *Game::getForeground()
	{
		return mForeground.get();
	}
	
	void Game::update(float dt)
	{
		if (mActiveObjects)
		{
			ObjectList::iterator iter;
			for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
			{
				iter->get()->update(dt);
			}
		}
		mCamera.update(dt);
	}

}
}
