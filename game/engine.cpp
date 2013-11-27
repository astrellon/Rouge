#include "engine.h"

#include <lua/lua_state.h>

#include <sstream>

#include <log/logger.h>

#include "game.h"
#include "tile.h"
#include "tile_type.h"
#include "race.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	Engine *Engine::sMainEngine = nullptr;
	const int Engine::LUA_ID = LUA_ID_ENGINE;
	const char *Engine::LUA_TABLENAME = LUA_TABLE_ENGINE;

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
		EventInterface *manager = new EventInterface();
		GameObjectEvent::setManager(manager);

		mTopLevelTileSet = new TileSet();
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

		GameObjectEvent::setManager(nullptr);
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
	Vector2f Engine::gridToWorld(int x, int y) const
	{
		return Vector2f(static_cast<float>(x) * mGridSize, static_cast<float>(y) * mGridSize);
	}
	Vector2i Engine::worldToGrid(const Vector2f &world) const
	{
		return Vector2i(floor(world.x * mGridSizeResp), floor(world.y * mGridSizeResp));
	}
	Vector2i Engine::worldToGrid(float x, float y) const
	{
		return Vector2i(floor(x * mGridSizeResp), floor(y * mGridSizeResp));
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
					mUsingTileSet[iter->first] = getTileSet(iter->first.c_str());
				}
			}
			mUsingTileSetNames.clear();
			mUsingTileSetDirty = false;
		}
	}

	void Engine::setGameHud(ui::GameHud *hud)
	{
		mGameHud = hud;
	}
	ui::GameHud *Engine::getGameHud() const
	{
		return mGameHud;
	}

	void Engine::setEditorHud(ui::EditorHud *hud)
	{
		mEditorHud = hud;
	}
	ui::EditorHud *Engine::getEditorHud() const
	{
		return mEditorHud;
	}

	Tile *Engine::getTile(const char *name)
	{
		std::string nameStr = name;
		std::string tileSetStr;
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
			base::Handle<TileSet> tileSet = mTopLevelTileSet;
			if (tileSetStr.size() > 0)
			{
				tileSet = getTileSet(tileSetStr.c_str());
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
			Tile *foundTile = nullptr;
			for (auto iter = mUsingTileSet.begin(); iter != mUsingTileSet.end(); ++iter)
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

	TileSet *Engine::getTileSet(const char *tileSetName)
	{
		return getDefinition<TileSet>(mTileSets, tileSetName, 1);
	}
	void Engine::addTileSet(TileSet *tileSet)
	{
		addDefinition<TileSet>(tileSet, mTileSets, tileSet->getName().c_str());
	}
	TileSetMap &Engine::getTileSets()
	{
		return mTileSets;
	}
	TileSet *Engine::getTopLevelTileSet()
	{
		return mTopLevelTileSet;
	}

	LuaState &Engine::getLua()
	{
		return mLua;
	}
	bool Engine::loadLuaEngine(const char *scriptName)
	{
		if (!mLua.loadFile(scriptName)) 
		{
			std::stringstream ss;
			ss << "ERROR: Unable to load engine script file '" << scriptName << '\'';
			mLua.logStack(ss.str().c_str());
			return false;
		}

		if (mLua.hasAmType("on_engine_load", LUA_TFUNCTION))
		{
			mLua.call(0, 0);
			mLua.pop(1);
		}
		TileSet *global = getTileSet("default:default");
		if (global == nullptr)
		{
			am_log("SET", "Default tile set not found!");
			mTopLevelTileSet = new TileSet();
			addTileSet(mTopLevelTileSet);
		}
		else
		{
			mTopLevelTileSet = global;
		}
		usingTileSet("default:default");
		Tile::setDefaultTile(getTile("no_tile"));
		clearUsingTileSet();
		return true;
	}

	bool Engine::newGame(const char *scenarioName)
	{
		//if (mLua.hasGlobalFunction("new_game"))
		if (mLua.hasAmType("new_game", LUA_TFUNCTION))
		{
			am::lua::LuaHandle handle(mLua, 1);
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
			am::lua::LuaHandle handle(mLua, 1);
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

	const char *Engine::getBaseDefinitionPath(int id) const
	{
		if (id == 1)
		{
			return "data/tilesets/";
		}
		return "data/";
	}

}
}
