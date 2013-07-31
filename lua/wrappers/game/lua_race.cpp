#include "lua_race.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/race.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The race class represents the information relating to a specific race. 
	 * This is mostly for general information such as name and potentially a lengthy description.
	 * It is also used as an identifier for the race of a Character.
	 */
	/**
	 * Creates a new race with the given race name.
	 *
	 * @param string race_name The race name that is used to identify this race.
	 */
	int Race_ctor(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			const char *raceName = lua_tostring(lua, 1);
			Race *race = new Race(raceName);
		
			wrapRefObject<Race>(lua, race);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string race_name");
	}
	/**
	 * TODO
	 */
	int Race_dtor(lua_State *lua)
	{
		return 0;
	}
	/**
	 * Compares this race against another race object.
	 *
	 * @param am.race rhs The other race object to compare with.
	 * @returns boolean True if the the they are the same race object.
	 */
	int Race_eq(lua_State *lua)
	{
		Race *lhs = castUData<Race>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.race");
		}
		Race *rhs = castUData<Race>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Race_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Race_ctor },
			{ "__gc",  Race_dtor },
			{ "__eq", Race_eq },
			{ "race_name", Race_race_name },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Race::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the race name identifier for this race.
	 *
	 * @returns string The race name.
	 */
	int Race_race_name(lua_State *lua)
	{
		Race *race = castUData<Race>(lua, 1);
		if (race)
		{
			lua_pushstring(lua, race->getRaceName());
			return 1;
		}
		return LuaState::expectedContext(lua, "race_name", "am.race");
	}

}
}
}
