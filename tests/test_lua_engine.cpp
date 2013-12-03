#include "test_lua_engine.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>

#include <game/engine.h>

#include <lua/wrappers/game/lua_engine.h>
#include <lua/wrappers/game/lua_tile.h>
#include <lua/wrappers/game/lua_tile_set.h>

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaEngine::testSimple()
	{
		lua::LuaState lua;

		game::Engine *prevEng = game::Engine::getEngine();
		game::Engine *eng = new game::Engine();
		game::Engine::setEngine(eng);
		
		int loadResult = lua.loadString(
			"function setGridSize(size)\n"
			"	am.engine.grid_size(size)\n"
			"end\n"
			"function getGridSize()\n"
			"	return am.engine.grid_size()\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		assert(lua.hasGlobalFunction("setGridSize"));
		lua.push(8);
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getGridSize"));
		lua_acall(lua, 0, 1);
		am_equals(8, lua_tointeger(lua, -1));
		lua.pop(2);

		game::Engine::setEngine(prevEng);
		delete eng;

		return true;
	}

	bool TestLuaEngine::testTiles()
	{
		lua::LuaState lua;

		game::Engine *prevEng = game::Engine::getEngine();
		game::Engine *eng = new game::Engine();
		eng->init();
		game::Engine::setEngine(eng);
		
		int loadResult = lua.loadString(
			"topSet = am.engine.top_level_tile_set()\n"
			"function addTile(tile, set)\n"
			"	if (set == nil) then\n"
			"		topSet:add_tile(tile)\n"
			"	else\n"
			"		set:add_tile(tile)\n"
			"	end\n"
			"end\n"
			"function getTile(tileName)\n"
			"	return am.engine.tile(tileName)\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		game::Tile *tile = new game::Tile("basicTile");
		TileSet *topLevel = eng->getTopLevelTileSet();
		assert(!topLevel->hasTile(tile));
		assert(eng->getTile("basicTile") == nullptr);

		assert(lua.hasGlobalFunction("getTile"));
		lua.push("basicTile");
		lua_acall(lua, 1, 1);
		assert(lua_isnil(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addTile"));
		lua::wrapRefObject<game::Tile>(lua, tile);
		lua_acall(lua, 1, 0);

		assert(topLevel->hasTile(tile));
		assert(topLevel->hasTile("basicTile"));

		assert(lua.hasGlobalFunction("getTile"));
		lua.push("basicTile");
		lua_acall(lua, 1, 1);
		game::Tile *user = lua::castUData<game::Tile>(lua, 1);
		assert(user == tile);
		lua.pop(1);

		game::Engine::setEngine(prevEng);
		delete eng;

		return true;
	}
}
}