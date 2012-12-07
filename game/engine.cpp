#include "engine.h"

//#include <util/json_value.h>
//using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

#include <sstream>
using namespace std;

#include <log/logger.h>

#include "game.h"
#include "tile.h"
#include "tile_type.h"
#include "race.h"

namespace am {
namespace game {

	Engine *Engine::sMainEngine = NULL;

	Engine::Engine() :
		mCurrentGame(NULL),
		mGridXSize(32.0f),
		mGridYSize(32.0f),
		mGridXSizeResp(1.0f / 32.0f),
		mGridYSizeResp(1.0f / 32.0f),
		mUsingTileSetDirty(true),
		mUnknownRace(new Race("Unknown"))
	{
	}
	Engine::~Engine() 
	{
		//deregisterTiles();
		if (mUnknownRace)
		{
			delete mUnknownRace;
			mUnknownRace = NULL;
		}
	}

	void Engine::init()
	{
		TileSet *global = getTileSetLua("global");
		if (global == NULL)
		{
			am_log("SET", "Global tile set not found!");
			mTopLevelTileSet = new TileSet();
			addTileSet(mTopLevelTileSet.get());
		}
		else
		{
			mTopLevelTileSet = global;
		}
	}
	void Engine::deinit()
	{
		if (mCurrentGame)
		{
			mCurrentGame->deinit();
		}
		LuaState::clearRegistered();
	}
	void Engine::update(float dt)
	{
		if (mCurrentGame.get())
		{
			mCurrentGame->update(dt);
		}
	}

	void Engine::setCurrentGame(Game *game)
	{
		mCurrentGame = game;
	}
	Game *Engine::getCurrentGame()
	{
		return mCurrentGame.get();
	}

	void Engine::usingTileSet(const char *tileSetName)
	{
		if (tileSetName)
		{
			string tileSetNameStr = tileSetName;
			if (mUsingTileSetNames.find(tileSetNameStr) == mUsingTileSetNames.end())
			{
				mUsingTileSetNames[tileSetNameStr] = 1;
				mUsingTileSetDirty = true;
			}
		}
	}
	void Engine::clearUsingTileSet()
	{
		mUsingTileSet.clear();
		mUsingTileSetNames.clear();
		mUsingTileSetDirty = false;
	}
	void Engine::checkUsingTileSet()
	{
		if (mUsingTileSetDirty)
		{
			UsingTileSet::iterator iter;
			TileSetMap::iterator tileIter;
			for (iter = mUsingTileSetNames.begin(); iter != mUsingTileSetNames.end(); ++iter)
			{
				tileIter = mTileSets.find(iter->first);
				if (tileIter != mTileSets.end())
				{
					mUsingTileSet[tileIter->first] = tileIter->second;
				}
				else
				{
					mUsingTileSet[iter->first] = getTileSetLua(iter->first.c_str());
				}
			}
			mUsingTileSetNames.clear();
			mUsingTileSetDirty = false;
		}
	}

	void Engine::setGameHud(GameHud *hud)
	{
		mGameHud = hud;
	}
	GameHud *Engine::getGameHud()
	{
		return mGameHud;
	}

	Tile *Engine::getTile(const char *name)
	{
		string nameStr = name;
		string tileSetStr;
		int tileSetIndex = nameStr.find("/");
		bool useSpecificTileSet = false;
		if (tileSetIndex >= 0)
		{
			useSpecificTileSet = true;
			tileSetStr = nameStr.substr(0, tileSetIndex);
			tileSetIndex++;
			nameStr = nameStr.substr(tileSetIndex, nameStr.size() - tileSetIndex);
		}

		if (useSpecificTileSet)
		{
			Handle<TileSet> tileSet = mTopLevelTileSet;
			if (tileSetStr.size() > 0)
			{
				tileSet = getTileSetLua(tileSetStr.c_str());
			}
			if (tileSet.get())
			{
				return tileSet->getTile(nameStr.c_str());
			}
			return NULL;
		}
		else
		{
			checkUsingTileSet();
			TileSetMap::iterator iter;
			Tile *foundTile = NULL;
			for (iter = mUsingTileSet.begin(); iter != mUsingTileSet.end(); ++iter)
			{
				foundTile = iter->second->getTile(nameStr.c_str());
				if (foundTile != NULL)
				{
					break;
				}
			}
			if (foundTile == NULL)
			{
				foundTile = mTopLevelTileSet->getTile(nameStr.c_str());
			}
			return foundTile;
		}

	}

	TileSet *Engine::getTileSetLua(const char *tileSetName)
	{
		if (!tileSetName || tileSetName[0] == '\0')
		{
			return mTopLevelTileSet.get();
		}
		string tileSetStr = tileSetName;
		TileSetMap::iterator iter = mTileSets.find(tileSetStr);
		if (iter != mTileSets.end())
		{
			return iter->second.get();
		}
		
		Handle<TileSet> tileSet(new TileSet(tileSetName));
		
		stringstream ss;
		ss << "data/tilesets/" << tileSetName << ".lua";
		LuaState lua(false);
		if (!lua.loadFile(ss.str().c_str()))
		{
			stringstream errss;
			errss << "Error loading tile set '" << tileSetName << "' definition using path '" << ss.str() << '\'';
			am_log("SET", errss);
			lua.logStack("SETLUA");
			lua.close();
			return NULL;
		}
		lua_getglobal(lua, "set");
		if (!lua_istable(lua, -1))
		{
			stringstream errss;
			errss << "Did not find global 'set' table: 'set' = " << lua_typename(lua, -1);
			am_log("SET", errss);
			lua.close();
			return NULL;
		}
		tileSet->loadDef(lua);
		mTileSets[tileSetStr] = tileSet;
		lua.close();
		return tileSet;
	}
	void Engine::addTileSet(TileSet *tileSet)
	{
		if (tileSet)
		{
			mTileSets[tileSet->getName()] = tileSet;
		}
	}
	TileSetMap &Engine::getTileSets()
	{
		return mTileSets;
	}
	TileSet *Engine::getTopLevelTileSet()
	{
		return mTopLevelTileSet.get();
	}

	void Engine::setEngine(Engine *engine)
	{
		sMainEngine = engine;
	}
	Engine *Engine::getEngine()
	{
		return sMainEngine;
	}

	void Engine::setGridXSize(float size)
	{
		if (size > -0.0001f && size < 0.0f)
		{
			size = -0.0001f;
		}
		if (size < 0.0001f && size >= 0.0f)
		{
			size = 0.0001f;
		}
		mGridXSize = size;
		mGridXSizeResp = 1.0f / size;
	}
	float Engine::getGridXSize() const
	{
		return mGridXSize;
	}
	float Engine::getGridXSizeResp() const
	{
		return mGridXSizeResp;
	}

	void Engine::setGridYSize(float size)
	{
		if (size > -0.0001f && size < 0.0f)
		{
			size = -0.0001f;
		}
		if (size < 0.0001f && size >= 0.0f)
		{
			size = 0.0001f;
		}
		mGridYSize = size;
		mGridYSizeResp = 1.0f / size;
	}
	float Engine::getGridYSize() const
	{
		return mGridYSize;
	}
	float Engine::getGridYSizeResp() const
	{
		return mGridYSizeResp;
	}

	Game *Engine::getGame()
	{
		if (sMainEngine)
		{
			return sMainEngine->getCurrentGame();
		}
		return NULL;
	}

	GameObject *Engine::getByGameId(const char *id) const
	{
		if (mCurrentGame.get())
		{
			return mCurrentGame->getByGameId(id);
		}
		return NULL;
	}
	void Engine::registerGameObject(GameObject *obj)
	{
		if (mCurrentGame.get())
		{
			mCurrentGame->registerGameObject(obj);
		}
	}
	void Engine::deregisterGameObject(GameObject *obj)
	{
		if (mCurrentGame.get())
		{
			mCurrentGame->deregisterGameObject(obj);
		}
	}

	bool Engine::addRace(Race *race)
	{
		if (race == NULL)
		{
			return false;
		}
		RaceMap::const_iterator iter = mRaces.find(race->getRaceName());
		if (iter == mRaces.end())
		{
			mRaces[race->getRaceName()] = race;
			return true;
		}
		return false;
	}
	bool Engine::removeRace(Race *race)
	{
		if (race == NULL)
		{
			return false;
		}
		RaceMap::const_iterator iter = mRaces.find(race->getRaceName());
		if (iter != mRaces.end())
		{
			mRaces.erase(iter);
			return true;
		}
		return false;
	}
	Race *Engine::getRace(const char *raceName)
	{
		if (raceName == NULL || raceName[0] == '\0')
		{
			return NULL;
		}
		RaceMap::iterator iter = mRaces.find(string(raceName));
		if (iter == mRaces.end())
		{
			return NULL;
		}
		return iter->second;
	}
	Race *Engine::getUnknownRace()
	{
		return mUnknownRace;
	}

	void Engine::addTileType(TileType *type)
	{
		if (type == NULL)
		{
			return;
		}
		mTileTypes[type->getName()] = type;
	}
	TileType *Engine::getTileType(const char *name)
	{
		return getTileType(string(name));
	}
	TileType *Engine::getTileType(const string &name)
	{
		TileTypeMap::iterator iter = mTileTypes.find(name);
		if (iter == mTileTypes.end())
		{
			return NULL;
		}
		return iter->second;
	}

}
}
