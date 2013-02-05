#include "test_lua_engine.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/engine.h>
using namespace am::game;

#include <lua/wrappers/game/lua_engine.h>
#include <lua/wrappers/game/lua_tile.h>
#include <lua/wrappers/game/lua_tile_set.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaEngine::testSimple() {
		LuaState lua;

		Engine *prevEng = Engine::getEngine();
		Engine *eng = new Engine();
		Engine::setEngine(eng);
		
		int loadResult = lua.loadString("Engine = import(\"Engine\")\n"
			"function setGridSize(x, y)\n"
			"	Engine.grid_size(x, y)\n"
			"end\n"
			"function getGridSize()\n"
			"	return Engine.grid_size()\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		assert(lua.hasGlobalFunction("setGridSize"));
		lua.push(8);
		lua.push(5);
		lua.call(2, 0);

		assert(lua.hasGlobalFunction("getGridSize"));
		lua.call(0, 2);
		equals(8, lua_tointeger(lua, -2));
		equals(5, lua_tointeger(lua, -1));
		lua.pop(2);

		Engine::setEngine(prevEng);
		delete eng;

		return true;
	}

	bool TestLuaEngine::testTiles() {
		LuaState lua;

		Engine *prevEng = Engine::getEngine();
		Engine *eng = new Engine();
		eng->init();
		Engine::setEngine(eng);
		
		int loadResult = lua.loadString("Engine = import(\"Engine\")\n"
			"Tile = import(\"Tile\")\n"
			"TileSet = import(\"TileSet\")\n"
			"topSet = Engine.top_level_tile_set()\n"
			"function addTile(tile, set)\n"
			"	if (set == nil) then\n"
			"		topSet:add_tile(tile)\n"
			"	else\n"
			"		set:add_tile(tile)\n"
			"	end\n"
			"end\n"
			"function getTile(tileName)\n"
			"	return Engine.tile(tileName)\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Tile *tile = new Tile("basicTile");
		TileSet *topLevel = eng->getTopLevelTileSet();
		assert(!topLevel->hasTile(tile));
		assert(eng->getTile("basicTile") == NULL);

		assert(lua.hasGlobalFunction("getTile"));
		lua.push("basicTile");
		lua.call(1, 1);
		assert(lua_isnil(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addTile"));
		wrapRefObject<Tile>(lua, tile);
		lua.call(1, 0);

		assert(topLevel->hasTile(tile));
		assert(topLevel->hasTile("basicTile"));

		assert(lua.hasGlobalFunction("getTile"));
		lua.push("basicTile");
		lua.call(1, 1);
		Tile *user = castUData<Tile>(lua, 1);
		assert(user == tile);
		lua.pop(1);

		Engine::setEngine(prevEng);
		delete eng;

		return true;
	}
}
}