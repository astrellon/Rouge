#include "lua_map_region.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/map_region.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * Stores a representation of a map region.
	 */
	/**
	 * Creates a new map region instance.
	 *
	 * @param integer width Width of the region.
	 * @param integer height Height of the region.
	 * @param integer [0] initial_value The initial value of each block in the region.
	 */
	int MapRegion_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (lua_isnum(lua, 1) && lua_isnum(lua, 2) && (args == 2 ||(args > 2 && lua_isnum(lua, 3))))
		{
			MapRegion *region = nullptr;
			if (args == 2)
			{
				region = new MapRegion(lua_tointeger(lua, 1), lua_tointeger(lua, 2));
			}
			else
			{
				region = new MapRegion(lua_tointeger(lua, 1), lua_tointeger(lua, 2), lua_tointeger(lua, 3));
			}
			wrapRefObject<MapRegion>(lua, region);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string partName, string [\"unknown_part\"] type");
	}
	/**
	 * Releases body part reference counter.
	 */
	int MapRegion_dtor(lua_State *lua)
	{
		MapRegion *part = castUData<MapRegion>(lua, 1);
		if (part)
		{
			part->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.map_region");
	}
	/**
	 * Compares if two body parts are the same object.
	 * @param am.map_region rhs The body part to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int MapRegion_eq(lua_State *lua)
	{
		MapRegion *lhs = castUData<MapRegion>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.map_region");
		}
		MapRegion *rhs = castUData<MapRegion>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int MapRegion_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", MapRegion_ctor },
			{ "__gc",  MapRegion_dtor },
			{ "__eq", MapRegion_eq },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, MapRegion::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

}
}
}
