#include "lua_data_map.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <util/data_map.h>
#include <util/data_number.h>
#include <util/data_array.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
using namespace am::util::data;

namespace am {
namespace lua {
namespace util {
namespace data {
	/**
	 * @class
	 * Simple wrapper for the util::data::Map class. Allows for storing
	 * and retrieving tables of primative data, including other tables, 
	 * arrays, numbers, booleans and strings.
	 */
	/**
	 * Creates a new util::data::Map with no data.
	 */
	int DataMap_ctor(lua_State *lua)
	{
		Map *map = new Map();
		wrapRefObject<Map>(lua, map);
		return 1;
	}
	/**
	 * Releases the reference counter on the util::data::Map.
	 */
	int DataMap_dtor(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			map->release();
		}
		return 0;
	}
	/**
	 * Compares this Map with another Map object.
	 *
	 * @param Map rhs The other Map to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int DataMap_eq(lua_State *lua)
	{
		Map *lhs = castUData<Map>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "Map");
		}
		Map *rhs = castUData<Map>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int DataMap_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", DataMap_ctor },
			{ "__gc", DataMap_dtor },
			{ "__eq", DataMap_eq },
			{ "at", DataMap_at },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Map::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the asset name currently used by this Map.
	 * An empty string indicates that no asset has been set.
	 *
	 * @returns string The asset name is use.
	 */
	/**
	 * Sets the asset name for this Map to use.
	 *
	 * @param string assetName The name of the asset to use.
	 * @returns Map This
	 */
	int DataMap_at(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			if (lua_isstr(lua, 2))
			{
				if (lua_gettop(lua) == 2)
				{
					IData *data = map->at(lua_tostring(lua, 2));
					if (data)
					{
						if (data->type() == Map::TYPE)
						{
							wrapRefObject<Map>(lua, dynamic_cast<Map *>(data));
							return 1;
						}
						if (data->type() == Array::TYPE)
						{
							wrapRefObject<Array>(lua, dynamic_cast<Array *>(data));
							return 1;
						}
						if (data->type() == Number::TYPE)
						{
							lua_pushnumber(lua, data->number());
							return 1;
						}
						if (data->type() == Boolean::TYPE)
						{
							lua_pushboolean(lua, data->boolean());
							return 1;
						}
						if (data->type() == String::TYPE)
						{
							lua_pushstring(lua, data->string());
							return 1;
						}
					}
					lua_pushnil(lua);
					return 1;
				}
				else
				{
					int luaType = lua_type(lua, 3);
					if (luaType == LUA_TNUMBER || luaType == LUA_TBOOLEAN || 
						luaType == LUA_TSTRING || luaType == LUA_TTABLE ||
						luaType == LUA_TNIL)
					{
						LuaState L(lua);
						IData *data = IData::fromLua(L, 3);
						if (data)
						{
							map->push(lua_tostring(lua, 2), data);
						}
						else
						{
							map->inner().erase(lua_tostring(lua, 2));
						}
						lua_first(lua);
					}
				}
			}
			return LuaState::expectedArgs(lua, "at", 2, "string key", "string key, primative value");
		}
		return LuaState::expectedContext(lua, "at", "Map");
	}

}
}
}
}
