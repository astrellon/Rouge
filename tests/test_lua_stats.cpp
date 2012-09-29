#include "test_lua_stats.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/stats.h>
using namespace am::game;

#include <lua/wrappers/lua_stats.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaStats::testSimple() {
		LuaState lua;
		
		assert(lua.loadString("Stats = import(\"Stats\")\n"
			"stats = Stats.new()\n"
			"function getBaseStat(key)\n"
			"	return stats:get_base_stat(key)\n"
			"end\n"
			"function setBaseStat(key, value)\n"
			"	stats:set_base_stat(key, value)\n"
			"end\n"
			"function getStat(key)\n"
			"	return stats:get_stat(key)\n"
			"end"));

		assert(lua.hasGlobalFunction("getBaseStat"));
		lua.push("health");
		lua.call(1, 1);
		equalsDelta(0.0f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("setBaseStat"));
		lua.push("health");
		lua.push(5.7f);
		lua.call(2, 0);

		assert(lua.hasGlobalFunction("getBaseStat"));
		lua.push("health");
		lua.call(1, 1);
		equalsDelta(5.7f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("getStat"));
		lua.push("health");
		lua.call(1, 1);
		equalsDelta(5.7f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		return true;
	}

}
}