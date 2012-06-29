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
#include "map.h"

using namespace am::util;

namespace am {
namespace base {

	Game::Game(Engine *engine) :
		//mMapFilename(""),
		mEngine(engine),
		mCurrentMap(NULL),
		mActiveObjects(NULL)
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

	Map *Game::getMap(const char *mapName)
	{
		return getMap(string(mapName));
	}
	Map *Game::getMap(const string &mapName)
	{
		MapMap::iterator iter = mMaps.find(mapName);
		if (iter != mMaps.end())
		{
			return iter->second.get();
		}

		stringstream ss;
		ss << "data/maps/" << mapName << ".ssff";
		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load map '" << mapName << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("MAP", errss);
			return NULL;
		}

		Handle<Map> map(new Map(mapName.c_str()));
		map->loadDef(loaded);

		mMaps[mapName] = map;

		return map.get();
	}

	Map *Game::getCurrentMap()
	{
		return mCurrentMap.get();
	}
	
	void Game::setCurrentMap(Map *map)
	{
		mBackground->clear();
		mItemLayer->clear();
		mCharacterLayer->clear();
		mForeground->clear();
		mCamera.followObject(NULL);

		mCurrentMap = map;
		if (map)
		{
			mBackground->addChild(map->getBackground());
			mForeground->addChild(map->getForeground());
			mActiveObjects = map->getObjects();

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
				errss << "Map (" << map->getName() << ") return a NULL object list.";
				am_log("MAP", errss);
			}
		}
	}
	void Game::setCurrentMap(const char *mapName)
	{
		setCurrentMap(getMap(string(mapName)));
	}
	void Game::setCurrentMap(const string &mapName)
	{
		setCurrentMap(getMap(mapName));
	}

	bool Game::addGameObject(GameObject *object)
	{
		if (mCurrentMap.get() == NULL)
		{
			am_log("MAP", "Unable to add game object to null current map");
			return false;
		}
		if (mCurrentMap->addGameObject(object))
		{
			mCharacterLayer->addChild(object);
			return true;
		}
		return false;
	}
	bool Game::removeGameObject(GameObject *object)
	{
		if (mCurrentMap.get() == NULL)
		{
			am_log("MAP", "Unable to remove game object from null current map");
			return false;
		}
		if (mCurrentMap->removeGameObject(object))
		{
			mCharacterLayer->removeChild(object);
			return true;
		}
		return false;
	}
	bool Game::hasGameObject(GameObject *object) const
	{
		if (mCurrentMap.get() == NULL)
		{
			am_log("MAP", "Unable to search for game object with null current map");
			return false;
		}
		return mCurrentMap->hasGameObject(object);
	}

	void Game::moveObjectToMap(GameObject *object, const char *mapName, float x, float y, bool setAsCurrent)
	{
		if (!object)
		{
			return;
		}
		moveObjectToMap(object, getMap(mapName), x, y, setAsCurrent);
	}
	void Game::moveObjectToMap(GameObject *object, Map *map, float x, float y, bool setAsCurrent)
	{
		if (!object || !map)
		{
			return;
		}
		Map *currentMap = object->getMap();
		if (currentMap)
		{
			if (currentMap->removeGameObject(object) && currentMap == mCurrentMap.get())
			{
				mCharacterLayer->removeChild(object);
			}
		}
		bool following = false;
		if (setAsCurrent)
		{
			following = mCamera.getFollowing() == object;
			setCurrentMap(map);
		}
		object->setMap(map);
		object->setLocation(x, y);
		if (following)
		{
			mCamera.followObject(object);
		}
		if (map)
		{
			if (map->addGameObject(object) && map == mCurrentMap.get())
			{
				mCharacterLayer->addChild(object);
			}
		}
	}
	void Game::moveObjectToMapGrid(GameObject *object, const char *mapName, int x, int y, bool setAsCurrent)
	{
		float gx = Engine::getEngine()->getGridXSize() * static_cast<float>(x);
		float gy = Engine::getEngine()->getGridYSize() * static_cast<float>(y);
		moveObjectToMap(object, mapName, gx, gy, setAsCurrent);
	}
	void Game::moveObjectToMapGrid(GameObject *object, Map *map, int x, int y, bool setAsCurrent)
	{
		float gx = Engine::getEngine()->getGridXSize() * static_cast<float>(x);
		float gy = Engine::getEngine()->getGridYSize() * static_cast<float>(y);
		moveObjectToMap(object, map, gx, gy, setAsCurrent);
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
