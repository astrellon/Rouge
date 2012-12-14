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
		
			Race_wrap(lua, race);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	void Race_wrap(lua_State *lua, Race *race)
	{
		Race ** udata = (Race **)lua_newuserdata(lua, sizeof(Race *));
		*udata = race;

		luaL_getmetatable(lua, Race_tableName);
		lua_setmetatable(lua, -2);
	}

	int Race_dtor(lua_State *lua)
	{
		/*Race *part = Check_Race(lua, 1);
		if (part)
		{
			delete part;
		}*/
		return 0;
	}

	int Race_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Race_ctor },
			{ "__gc",  Race_dtor },
			{ "get_race_name", Race_get_race_name },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Race_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	Race *Check_Race(lua_State *lua, int n)
	{
		return *(Race **)luaL_checkudata(lua, n, Race_tableName);
	}

	int Race_get_race_name(lua_State *lua)
	{
		Race *race = Check_Race(lua, 1);
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
