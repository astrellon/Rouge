#include "test_lua_tile_set.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile.h>
#include <game/tile_set.h>
using namespace am::game;

#include <lua/wrappers/game/lua_tile.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaTileSet::testSimple() {
		LuaState lua;
		
		assert(lua.loadString("TileSet = import(\"TileSet\")\n"
			"set = TileSet.new(\"testTileSet\")\n"
			"function getName()\n"
			"	return set:name()\n"
			"end\n"
			"function getFullName()\n"
			"	return set:full_name()\n"
			"end\n"
			"function setFullName(name)\n"
			"	set:full_name(name)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("testTileSet", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setFullName"));
		lua.push("New Full Name");
		lua_acall(lua, 1, 0);
		
		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("New Full Name", lua_tostring(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaTileSet::testTiles() {
		LuaState lua;
		
		assert(lua.loadString("TileSet = import(\"TileSet\")\n"
			"Tile = import(\"Tile\")\n"
			"set = TileSet.new(\"testTileSet\")\n"
			"function addTile(tile)\n"
			"	set:add_tile(tile)\n"
			"end\n"
			"function removeTile(tile)\n"
			"	set:remove_tile(tile)\n"
			"end\n"
			"function getTile(tileName)\n"
			"	return set:tile(tileName)\n"
			"end\n"
			"function hasTile(tile)\n"
			"	return set:has_tile(tile)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("hasTile"));
		lua.push("myTile");
		lua_acall(lua, 1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		Tile *tile = new Tile("myTile");

		assert(lua.hasGlobalFunction("addTile"));
		wrapRefObject<Tile>(lua, tile);
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("hasTile"));
		lua.push("myTile");
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);
		
		assert(lua.hasGlobalFunction("hasTile"));
		wrapRefObject<Tile>(lua, tile);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getTile"));
		lua.push("myTile");
		lua_acall(lua, 1, 1);
		lua_getmetatable(lua, -1);
		Tile *get = castUData<Tile>(lua, 1);
		lua.pop(1);
		assert(tile == get);

		assert(lua.hasGlobalFunction("removeTile"));
		lua.push("myTile");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("hasTile"));
		lua.push("myTile");
		lua_acall(lua, 1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		return true;
	}

}
}