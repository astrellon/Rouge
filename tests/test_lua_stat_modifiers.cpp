#include "test_lua_stat_modifiers.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/stat_modifiers.h>
using namespace am::game;

#include <lua/wrappers/game/lua_stat_modifiers.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaStatModifiers::testSimple() {
		LuaState lua;
		StatModifiers mods;
		
		int loadResult = lua.loadString("StatModifiers = import(\"StatModifiers\")\n"
			"stats = StatModifiers.new()\n"
			"function addModifier(stat, value, type, magical)\n"
			"	if (magical == nil) then\n"
			"		stats:add(stat, value, type)\n"
			"	else\n"
			"		stats:add(stat, value, type, magical)\n"
			"	end\n"
			"end\n"
			"function removeModifier(stat, value, type, magical)\n"
			"	if (magical == nil) then\n"
			"		stats:remove(stat, value, type)\n"
			"	else\n"
			"		stats:remove(stat, value, type, magical)\n"
			"	end\n"
			"end\n"
			"function calculateStat(stat, value)\n"
			"	return stats:calculate_stat(stat, value)\n"
			"end\n");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		assert(lua.hasGlobalFunction("calculateStat"));
		lua.push("health");
		lua.push(5.0f);
		lua.call(2, 1);
		equalsDelta(5.0f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("addModifier"));
		lua.push("health");
		lua.push(2.5f);
		lua.push("*");
		lua.call(3, 0);

		assert(lua.hasGlobalFunction("calculateStat"));
		lua.push("health");
		lua.push(5.0f);
		lua.call(2, 1);
		equalsDelta(12.5f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("addModifier"));
		lua.push("health");
		lua.push(2.5f);
		lua.push("+");
		lua.call(3, 0);

		assert(lua.hasGlobalFunction("calculateStat"));
		lua.push("health");
		lua.push(5.0f);
		lua.call(2, 1);
		equalsDelta(15.0f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeModifier"));
		lua.push("health");
		lua.push(2.5f);
		lua.push("*");
		lua.call(3, 0);

		assert(lua.hasGlobalFunction("calculateStat"));
		lua.push("health");
		lua.push(5.0f);
		lua.call(2, 1);
		equalsDelta(7.5f, lua_tonumber(lua, -1), 0.0001f);
		lua.pop(1);

		return true;
	}

	bool TestLuaStatModifiers::testSimple2() {
		LuaState lua;
		StatModifiers mods;
		
		int loadResult = lua.loadString("StatModifiers = import(\"StatModifiers\")\n"
			"mods = StatModifiers.new()\n"
			"base_health = 5\n"
			"mods:add(\"health\", 4, \"+\")\n"
			"am_log(\"CH: \" .. mods:calculate_stat(\"health\", base_health))\n"
			"mods:add(\"health\", 3, \"*\")\n"
			"am_log(\"CH: \" .. mods:calculate_stat(\"health\", base_health))\n"
			"mods:add(\"health\", 7, \"=\")\n"
			"am_log(\"CH: \" .. mods:calculate_stat(\"health\", base_health))\n"
		);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		return true;
	}
}
}