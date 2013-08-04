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

	Engine *Engine::sMainEngine = nullptr;
	const int Engine::LUA_ID = 0x06;
	const char *Engine::LUA_TABLENAME = "am_game_Engine";

	Engine::Engine() :
		mCurrentGame(nullptr),
		mGridSize(32.0f),
		mGridSizeResp(1.0f / 32.0f),
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
			mUnknownRace = nullptr;
		}
	}

	void Engine::init()
	{
		TileSet *global = getTileSetLua("global");
		if (global == nullptr)
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
		
		mTileSets.clear();
		mTileTypes.clear();
		
		mLua.close();
	}
	void Engine::update(float dt)
	{
		if (mCurrentGame.get())
		{
			mCurrentGame->update(dt);
			// Should likely be in it's own callback.
			mCurrentGame->onGameTick();
		}
	}

	Vector2f Engine::gridToWorld(const Vector2i &grid) const
	{
		return Vector2f(static_cast<float>(grid.x) * mGridSize, static_cast<float>(grid.y) * mGridSize);
	}
	Vector2i Engine::worldToGrid(const Vector2f &world) const
	{
		return Vector2i(floor(world.x * mGridSizeResp), floor(world.y * mGridSizeResp));
	}

	void Engine::setCurrentGame(Game *game)
	{
		mCurrentGame = game;
	}
	Game *Engine::getCurrentGame()
	{
		return mCurrentGame;
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
			return nullptr;
		}
		else
		{
			checkUsingTileSet();
			TileSetMap::iterator iter;
			Tile *foundTile = nullptr;
			for (iter = mUsingTileSet.begin(); iter != mUsingTileSet.end(); ++iter)
			{
				foundTile = iter->second->getTile(nameStr.c_str());
				if (foundTile != nullptr)
				{
					break;
				}
			}
			if (foundTile == nullptr)
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
			return nullptr;
		}
		lua_getglobal(lua, "set");
		if (!lua_istable(lua, -1))
		{
			stringstream errss;
			errss << "Did not find global 'set' table: 'set' = " << lua_typename(lua, -1);
			am_log("SET", errss);
			lua.close();
			return nullptr;
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

	LuaState &Engine::getLua()
	{
		return mLua;
	}
	bool Engine::loadLuaEngine(const char *scriptName)
	{
		if (!mLua.loadFile(scriptName)) 
		{
			stringstream ss;
			ss << "ERROR: Unable to load engine script file '" << scriptName << '\'';
			mLua.logStack(ss.str().c_str());
			return false;
		}

		if (mLua.hasAmType("on_engine_load", LUA_TFUNCTION))
		{
			mLua.call(0, 0);
			mLua.pop(1);
		}
		return true;
	}

	bool Engine::newGame(const char *scenarioName)
	{
		//if (mLua.hasGlobalFunction("new_game"))
		if (mLua.hasAmType("new_game", LUA_TFUNCTION))
		{
			LuaHandle handle(mLua, 1);
			try
			{
				mLua.push(scenarioName);
				mLua.call(1, 1);
				int result = mLua.toInteger();
				if (result != 1)
				{
					am_log("LUAERR", "Error starting scenario");
				}
			}
			catch(std::runtime_error err)
			{
				am_log("LUAERR", err.what());
				return false;
			}
		}
		else
		{
			am_log("ERROR", "Main engine script does not have a 'new_game' function");
			return false;
		}
		if (mCurrentGame)
		{
			mCurrentGame->setScenarioName(scenarioName);
		}
		return true;
	}

	bool Engine::loadGame(const char *saveName)
	{
		if (mLua.hasAmType("load_game", LUA_TFUNCTION))
		{
			LuaHandle handle(mLua, 1);
			try
			{
				mLua.push(saveName);
				mLua.call(1, 1);
				int result = mLua.toInteger();
				if (result != 1)
				{
					am_log("LUAERR", "Error loading scenario");
				}
			}
			catch(std::runtime_error err)
			{
				am_log("LUAERR", err.what());
				return false;
			}
		}
		else
		{
			am_log("ERROR", "Main engine script does not have a 'load_game' function");
			return false;
		}
		return true;
	}

	void Engine::setEngine(Engine *engine)
	{
		sMainEngine = engine;
	}
	Engine *Engine::getEngine()
	{
		return sMainEngine;
	}

	void Engine::setGridSize(float size)
	{
		if (size > -0.0001f && size < 0.0f)
		{
			size = -0.0001f;
		}
		if (size < 0.0001f && size >= 0.0f)
		{
			size = 0.0001f;
		}
		mGridSize = size;
		mGridSizeResp = 1.0f / size;
	}
	float Engine::getGridSize() const
	{
		return mGridSize;
	}
	float Engine::getGridSizeResp() const
	{
		return mGridSizeResp;
	}

	float Engine::gridSize()
	{
		if (sMainEngine)
		{
			return sMainEngine->getGridSize();
		}
		return 0.0f;
	}
	float Engine::gridSizeResp()
	{
		if (sMainEngine)
		{
			return sMainEngine->getGridSizeResp();
		}
		return 0.0f;
	}

	Game *Engine::getGame()
	{
		if (sMainEngine)
		{
			return sMainEngine->getCurrentGame();
		}
		return nullptr;
	}

	GameObject *Engine::getGameObject(const char *id) const
	{
		if (mCurrentGame.get())
		{
			return mCurrentGame->getGameObject(id);
		}
		return nullptr;
	}
	bool Engine::registerGameObject(GameObject *obj)
	{
		if (mCurrentGame.get())
		{
			return mCurrentGame->registerGameObject(obj);
		}
		return false;
	}
	void Engine::deregisterGameObject(const char *id)
	{
		if (mCurrentGame.get())
		{
			mCurrentGame->deregisterGameObject(id);
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
		if (race == nullptr)
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
		if (race == nullptr)
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
		if (raceName == nullptr || raceName[0] == '\0')
		{
			return nullptr;
		}
		RaceMap::iterator iter = mRaces.find(string(raceName));
		if (iter == mRaces.end())
		{
			return nullptr;
		}
		return iter->second;
	}
	Race *Engine::getUnknownRace()
	{
		return mUnknownRace;
	}

	void Engine::addTileType(TileType *type)
	{
		if (type == nullptr)
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
			return nullptr;
		}
		return iter->second;
	}

}
}
