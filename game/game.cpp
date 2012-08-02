#include "game.h"

#include <util/json_value.h>
using namespace am::util;

#include <log/logger.h>

#include <sstream>
#include <stdio.h>

#include <gfx/gfx_layer.h>
#include <gfx/gfx_engine.h>

#include <ui/mouse_manager.h>
#include <ui/ui_button.h>
#include <ui/ui_game_hud.h>
#include <ui/ui_inspector.h>

#include "engine.h"
#include "map.h"

namespace am {
namespace game {

	Game::Game(Engine *engine) :
		//mMapFilename(""),
		mEngine(engine),
		mCurrentMap(NULL),
		mActiveObjects(NULL)
	{
		mGameLayer = new Layer();
		mGameLayer->setName("Game->GameLayer");
		mGameLayer->setInteractive(true);
		//GfxEngine::getEngine()->getRootLayer()->addChild(mGameLayer.get());

		mBackground = new Layer();
		mBackground->setName("Background");
		mBackground->setInteractive(false);
		mGameLayer->addChild(mBackground.get());

		mItemLayer = new Layer();
		mItemLayer->setName("ItemLayer");
		mItemLayer->setInteractive(true);
		mGameLayer->addChild(mItemLayer.get());

		mCharacterLayer = new Layer();
		mCharacterLayer->setName("CharacterLayer");
		mCharacterLayer->setInteractive(true);
		mGameLayer->addChild(mCharacterLayer.get());

		mForeground = new Layer();
		mForeground->setName("Foreground");
		mForeground->setInteractive(false);
		mGameLayer->addChild(mForeground.get());

		MouseManager::getManager()->addEventListener(MOUSE_UP, this);
	}
	Game::~Game()
	{
		MouseManager::getManager()->removeEventListener(MOUSE_UP, this);
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

	void Game::onEvent(MouseEvent *e)
	{
		if (!mCurrentMap.get())
		{
			return;
		}

		am::math::TransformLite &trans = GfxEngine::getEngine()->getGameLayer()->getTransform();
		float localX = static_cast<float>(e->getMouseX()) - trans.getX();
		float localY = static_cast<float>(e->getMouseY()) - trans.getY();
		int gridX = static_cast<int>(localX * Engine::getEngine()->getGridXSizeResp());
		int gridY = static_cast<int>(localY * Engine::getEngine()->getGridYSizeResp());
		
		if (gridX < 0 || gridY < 0 || 
			gridX >= mCurrentMap->getMapWidth() ||
			gridY >= mCurrentMap->getMapHeight())
		{
			return;
		}

		GameHud *gameHud = Engine::getEngine()->getGameHud();
		if (gameHud)
		{
			Tile *tile = mCurrentMap->getTile(gridX, gridY);
			Inspector *inspector = gameHud->getInspector();
			inspector->setTile(tile);

			inspector->clearGameObjects();
			if (mActiveObjects)
			{
				ObjectList::iterator iter;
				for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
				{
					GameObject *obj = iter->get();
					float x = obj->getPositionX();
					float y = obj->getPositionY();
					if (localX >= x && localX >= y &&
						localX < x + obj->getWidth() &&
						localY < y + obj->getHeight())
					{
						inspector->addGameObject(obj);
					}
				}
			}
		}
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
			if (strcmp(object->getGameObjectTypeName(), "character") == 0)
			{
				mCharacterLayer->addChild(object);
			}
			else
			{
				mItemLayer->addChild(object);
			}
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
			if (strcmp(object->getGameObjectTypeName(), "character") == 0)
			{
				mCharacterLayer->removeChild(object);
			}
			else
			{
				mItemLayer->removeChild(object);
			}
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
