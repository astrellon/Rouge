#include "lua_debug.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace lua {
namespace debug {
	/**
	 * @class
	 * A helper class for debugging Lua code.
	 */
	int Debug_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "log", Debug_log },
			{ "equals", Debug_equals },
			{ "not_equals", Debug_not_equals },
			{ "print_stack", Debug_print_stack },
			{ "empty", Debug_empty },
			{ nullptr, nullptr }
		};

		lua_pushstring(lua, "debug");
		lua_newtable(lua);

		luaL_setfuncs(lua, regs, 0);
		lua_settable(lua, -3);

		return 0;
	}

	/**
	 * @static
	 * Takes a variable number of arguments and prints them to the logger.
	 * 
	 * @param ... args A variable number of Lua objects to log.
	 */
	inline int Debug_log(lua_State *lua)
	{
		return LuaState::luaAmLog(lua);
	}

	/**
	 * @static
	 * Does an assert for checking that the values are equal.
	 * If they are not an exception is thrown in the internal game code.
	 * This does not return a value as if there is an exception the Lua
	 * code will stop executing.
	 *
	 * @param * obj1 The first object
	 * @param * obj2 The second object
	 */
	inline int Debug_equals(lua_State *lua)
	{
		return LuaState::luaEquals(lua);
	}

	/**
	 * @static
	 * Does an assert for checking that the values are not equal.
	 * If they are not an exception is thrown in the internal game code.
	 * This does not return a value as if there is an exception the Lua
	 * code will stop executing.
	 *
	 * @param * obj1 The first object
	 * @param * obj2 The second object
	 */
	inline int Debug_not_equals(lua_State *lua)
	{
		return LuaState::luaNotEquals(lua);
	}

	/**
	 * @static
	 * Prints to console the current Lua stack.
	 */
	inline int Debug_print_stack(lua_State *lua)
	{
		return LuaState::luaPrintStack(lua);
	}

	/**
	 * @static
	 * Returns true if the given table is empty.
	 *
	 * @param table input The table to check.
	 * @returns boolean True if the table is empty.
	 */
	/**
	 * @static
	 * Returns true if the given string is empty.
	 *
	 * @param string input The string to check.
	 * @returns boolean True if the string is empty.
	 */
	int Debug_empty(lua_State *lua)
	{
		int type = lua_type(lua, 1);
		if (type == LUA_TTABLE)
		{

			lua_pushnil(lua);
			lua_pushboolean(lua, lua_next(lua, -2) == 0);
			return 1;
		}
		if (type == LUA_TSTRING)
		{
			const char *str = lua_tostring(lua, 1);
			lua_pushboolean(lua, !str || str[0] == '\0');
			return 1;
		}
		lua_pushboolean(lua, true);
		return 1;
	}

}
}
}
