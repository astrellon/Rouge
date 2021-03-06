#include "test_lua_tile_type.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>

#include <game/tile_type.h>

#include <lua/wrappers/game/lua_tile_type.h>

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaTileType::testSimple() 
	{
		lua::LuaState lua;
		
		assert(lua.loadString(
			"type = am.tile_type.new(\"testType\", \"Test Type\")\n"
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
		am_equalsStr("testType", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("Test Type", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setFullName"));
		lua.push("New Test Type");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("New Test Type", lua_tostring(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaTileType::testStatics()
	{
		lua::LuaState lua;
		
		assert(lua.loadString(
			"function addTileType(tile)\n"
			"	am.tile_type.add_tile_type(tile)\n"
			"end\n"
			"function getTileType(tileName)\n"
			"	return am.tile_type.tile_type(tileName)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getTileType"));
		lua.push("testTileType");
		lua_acall(lua, 1, 1);
		assert(lua_isnil(lua, -1));
		lua.pop(1);

		base::Handle<game::TileType> tileType(new game::TileType("testTileType", "Test Tile Type"));
		assert(lua.hasGlobalFunction("addTileType"));
		lua::wrapObject<game::TileType>(lua, tileType);
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