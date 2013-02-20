#include "test_lua_stats.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/stats.h>
using namespace am::game;

#include <lua/wrappers/game/lua_stats.h>
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
			"stats:add_modifier(\"strength\", 7.8, true)\n"
			"stats:add_modifier(\"dex\", 3.2)\n"
			"function getBaseStat(key)\n"
			"	return stats:get_base_stat(key)\n"
			"end\n"
			"function setBaseStat(key, value)\n"
			"	stats:set_base_stat(key, value)\n"
			"end\n"
			"function getStat(key)\n"
			"	return stats:get_stat(key)\n"
			"end\n"
			"function addModifier(stat, value, type, magical)\n"
			"	if (magical == nil) then\n"
			"		stats:add_modifier(stat, value, type)\n"
			"	else\n"
			"		stats:add_modifier(stat, value, type, magical)\n"
			"	end\n"
			"end\n"
			"function removeModifier(stat, value, type, magical)\n"
			"	if (magical == nil) then\n"
			"		stats:remove_modifier(stat, value, type)\n"
			"	else\n"
			"		stats:remove_modifier(stat, value, type, magical)\n"
			"	end\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getBaseStat"));
		lua.push("health");
		lua_acall(lua, 1, 1);
		equalsDelta(0.0f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("setBaseStat"));
		lua.push("health");
		lua.push(5.7f);
		lua_acall(lua, 2, 0);

		assert(lua.hasGlobalFunction("getBaseStat"));
		lua.push("health");
		lua_acall(lua, 1, 1);
		equalsDelta(5.7f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("getStat"));
		lua.push("health");
		lua_acall(lua, 1, 1);
		equalsDelta(5.7f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("addModifier"));
		lua.push("health");
		lua.push(10.0f);
		lua.push("*");
		lua.pushnil();
		lua_acall(lua, 4, 0);

		assert(lua.hasGlobalFunction("getStat"));
		lua.push("health");
		lua_acall(lua, 1, 1);
		equalsDelta(57.0f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("addModifier"));
		lua.push("health");
		lua.push(3.0f);
		lua.push("+");
		lua.pushnil();
		lua_acall(lua, 4, 0);

		assert(lua.hasGlobalFunction("getStat"));
		lua.push("health");
		lua_acall(lua, 1, 1);
		equalsDelta(60.0f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeModifier"));
		lua.push("health");
		lua.push(10.0f);
		lua.push("*");
		lua.pushnil();
		lua_acall(lua, 4, 0);

		assert(lua.hasGlobalFunction("getStat"));
		lua.push("health");
		lua_acall(lua, 1, 1);
		equalsDelta(8.7f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		return true;
	}

}
}