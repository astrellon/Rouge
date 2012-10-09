#include "test_lua_tile.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile.h>
#include <game/tile_type.h>
using namespace am::game;

#include <lua/wrappers/lua_tile_type.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaTile::testSimple() {
		LuaState lua;
		
		assert(lua.loadString("Tile = import(\"Tile\")\n"
			"tile = Tile.new(\"testTile\")\n"
			"function getName()\n"
			"	return tile:get_name()\n"
			"end\n"
			"function setName(name)\n"
			"	return tile:set_name(name)\n"
			"end\n"
			"function getFullName()\n"
			"	return tile:get_full_name()\n"
			"end\n"
			"function setFullName(name)\n"
			"	tile:set_full_name(name)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getName"));
		lua.call(0, 1);
		equalsStr("testTile", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setName"));
		lua.push("newTestTile");
		lua.call(1, 0);

		assert(lua.hasGlobalFunction("getName"));
		lua.call(0, 1);
		equalsStr("newTestTile", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setFullName"));
		lua.push("New Full Name");
		lua.call(1, 0);
		
		assert(lua.hasGlobalFunction("getFullName"));
		lua.call(0, 1);
		equalsStr("New Full Name", lua_tostring(lua, -1));
		lua.pop(1);

		return true;
	}

}
}