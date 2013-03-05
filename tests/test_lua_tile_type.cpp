#include "test_lua_tile_type.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile_type.h>
using namespace am::game;

#include <lua/wrappers/game/lua_tile_type.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaTileType::testSimple() {
		LuaState lua;
		
		assert(lua.loadString("TileType = import(\"TileType\")\n"
			"type = TileType.new(\"testType\", \"Test Type\")\n"
			"function getName()\n"
			"	return type:name()\n"
			"end\n"
			"function getFullName()\n"
			"	return type:full_name()\n"
			"end\n"
			"function setFullName(name)\n"
			"	type:full_name(name)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		equalsStr("testType", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		equalsStr("Test Type", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setFullName"));
		lua.push("New Test Type");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		equalsStr("New Test Type", lua_tostring(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaTileType::testStatics() {
		LuaState lua;
		
		assert(lua.loadString("TileType = import(\"TileType\")\n"
			"function addTileType(tile)\n"
			"	TileType.add_tile_type(tile)\n"
			"end\n"
			"function getTileType(tileName)\n"
			"	return TileType.tile_type(tileName)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getTileType"));
		lua.push("testTileType");
		lua_acall(lua, 1, 1);
		assert(lua_isnil(lua, -1));
		lua.pop(1);

		TileType *tileType = new TileType("testTileType", "Test Tile Type");
		assert(lua.hasGlobalFunction("addTileType"));
		wrapObject<TileType>(lua, tileType);
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getTileType"));
		lua.push("testTileType");
		lua_acall(lua, 1, 1);
		assert(!lua_isnil(lua, -1));
		lua.pop(1);

		return true;
	}

}
}