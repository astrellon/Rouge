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
		//GfxEngine::getGfxEngine()->getRootLayer()->addChild(mGameLayer.get());

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
			am_log("SCREEN", errss.str().c_str());
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
				am_log("SCR", errss.str().c_str());
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

	void Game::addGameObject(GameObject *object)
	{
		if (object == NULL)
		{
			return;
		}
		if (mActiveObjects)
		{
			ObjectList::iterator iter = findGameObject(object);
			if (iter == mActiveObjects->end())
			{
				mCharacterLayer->addChild(object);
				mActiveObjects->push_back(object);
			}
		}
		else
		{
			am_log("SCR", "Unable to add game object to null active list");
		}
	}
	void Game::removeGameObject(GameObject *object)
	{
		if (object == NULL)
		{
			return;
		}
		if (mActiveObjects)
		{
			ObjectList::iterator iter = findGameObject(object);
			if (iter != mActiveObjects->end())
			{
				mCharacterLayer->removeChild(object);
				mActiveObjects->erase(iter);
			}
		}
		else
		{
			am_log("SCR", "Unable to remove game object from null active list");
		}
	}
	bool Game::hasGameObject(GameObject *object) const
	{
		if (object == NULL)
		{
			return false;
		}
		if (mActiveObjects)
		{
			ObjectList::iterator iter = findGameObject(object);
			return iter != mActiveObjects->end();
		}
		am_log("SCR", "Unable to search for game object with null active list");
		return false;
	}
	ObjectList::iterator Game::findGameObject(GameObject *object) const
	{
		ObjectList::iterator iter;
		for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
		{
			if (iter->get() == object)
			{
				break;
			}
		}
		return iter;
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
