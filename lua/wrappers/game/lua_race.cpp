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

	int Race_ctor(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			const char *raceName = lua_tostring(lua, -1);
			Race *race = new Race(raceName);
		
			wrapObject<Race>(lua, race);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Race_dtor(lua_State *lua)
	{
		return 0;
	}

	int Race_eq(lua_State *lua)
	{
		Race *lhs = castUData<Race>(lua, 1);
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
			{ "get_race_name", Race_get_race_name },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Race::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int Race_get_race_name(lua_State *lua)
	{
		Race *race = castUData<Race>(lua, 1);
		if (race)
		{
			lua_pushstring(lua, race->getRaceName());
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
