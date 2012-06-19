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
		GfxEngine::getGfxEngine()->getRootLayer()->addChild(mGameLayer.get());

		mBackground = new Layer();
		mGameLayer->addChild(mBackground.get());

		mItemLayer = new Layer();
		mGameLayer->addChild(mItemLayer.get());

		mCharacterLayer = new Layer();
		mGameLayer->addChild(mCharacterLayer.get());

		mForeground = new Layer();
		mGameLayer->addChild(mForeground.get());
	}
	Game::~Game()
	{
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
		setCurrentScreen(mEngine->getScreen(string(screenName)));
	}
	void Game::setCurrentScreen(const string &screenName)
	{
		setCurrentScreen(mEngine->getScreen(screenName));
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
