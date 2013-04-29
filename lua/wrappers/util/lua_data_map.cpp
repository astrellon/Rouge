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
	 * Compares this data map with another data map object.
	 *
	 * @param DataMap rhs The other data map to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int DataMap_eq(lua_State *lua)
	{
		Map *lhs = castUData<Map>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "DataMap");
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
			{ "__len", DataMap_len },

			{ "at", DataMap_at },
			{ "each", DataMap_each },
			{ "remove", DataMap_remove },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Map::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the value associated with the given key.
	 * 
	 * A primative is a number, string, boolean, nil or table of primatives.
	 * 
	 * @param string key The key to look up.
	 * @returns primative The value.
	 */
	/**
	 * Sets the value at the given key.
	 * 
	 * A primative is a number, string, boolean, nil or table of primatives.
	 *
	 * @param string key The key to set.
	 * @param primative value The value to set.
	 * @returns DataMap This
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
					if (!pushData(lua, data))
					{
						lua_pushnil(lua);
					}
					return 1;
				}
				else
				{
					int luaType = lua_type(lua, 3);
					if (IS_LUA_PRIMATIVE(luaType))
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
		return LuaState::expectedContext(lua, "at", "DataMap");
	}

	/**
	 * Returns the number of key values in the data map.
	 * This is an implementation of the __len metamethod as such the
	 * Lua # operator can be used to get the size.
	 *
	 * @returns integer The number of key values in the data map.
	 */
	int DataMap_len(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			lua_pushinteger(lua, map->inner().size());
			return 1;
		}
		return LuaState::expectedContext(lua, "__len", "DataMap");
	}

	/**
	 * Removes the keys given list of strings.
	 * This takes a variable number of arguments which will be validated
	 * as being strings before a key is removed.
	 * It is not considered an error to remove a key that does not exist.
	 *
	 * <pre>
	 * DataMap = import("DataMap")
	 * map = DataMap.new()
	 * map:at("num", 5)
	 * map:at("str", "String")
	 * map:at("bool", true)
	 * 
	 * am_log("Size 1: " .. #map)	-- Outputs: Size 1: 3
	 * map:remove("num", "bool")
	 * 
	 * am_log("Size 2: " .. #ma)	-- Outputs: Size 2: 1
	 * </pre>
	 * 
	 * @param string... keys The keys to remove.
	 * @returns DataMap This
	 */
	int DataMap_remove(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			int args = lua_gettop(lua);
			if (args > 1)
			{
				bool valid = true;
				// Make sure that all the arguments are strings before removing anything.
				for (int n = 2; n < args; ++n)
				{
					if (!lua_isstr(lua, n))
					{
						valid = false;
						break;
					}
				}
				if (valid)
				{
					for (int n = 2; n < args; ++n)
					{
						map->remove(lua_tostring(lua, n));
					}
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "remove","string key ...");
		}
		return LuaState::expectedContext(lua, "remove", "DataMap");
	}

	/**
	 * The each function provides similar functionality to that of the pairs function.
	 * The each function takes a function as it's argument and that function will be
	 * called once per key/value in the data map. If that function returns false
	 * then the loop is stopped.
	 *
	 * <pre>
	 * DataMap = import("DataMap")
	 * map = DataMap.new()
	 * map:at("num", 5)
	 * map:at("str", "String")
	 * map:at("bool", true)
	 * map:each(function(key, value)\n
	 *     am_log(key .. " = ", value)
	 * end)
	 * -- The output order is not gurrantied
	 * -- num = 5
	 * -- str = "String"
	 * -- bool = true
	 * </pre>
	 *
	 * @param function handler The each function handler.
	 * @returns DataMap This
	 */
	int DataMap_each(lua_State *lua)
	{
		Map *map = castUData<Map>(lua, 1);
		if (map)
		{
			if (lua_isfunction(lua, 2))
			{
				for (auto iter = map->begin(); iter != map->end(); ++iter)
				{
					lua_pushvalue(lua, 2);
					lua_pushstring(lua, iter->first.c_str());
					if (!pushData(lua, iter->second.get()))
					{
						lua_pushnil(lua);
					}
					lua_call(lua, 2, 1);
					if (lua_isbool(lua, -1) && !lua_tobool(lua, -1))
					{
						break;
					}
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "each", "function handler");
		}
		return LuaState::expectedContext(lua, "each", "DataMap");
	}

	bool pushData(lua_State *lua, am::util::data::IData *data)
	{
		if (data)
		{
			if (data->type() == Map::TYPE)
			{
				wrapRefObject<Map>(lua, dynamic_cast<Map *>(data));
				return true;
			}
			if (data->type() == Array::TYPE)
			{
				wrapRefObject<Array>(lua, dynamic_cast<Array *>(data));
				return true;
			}
			if (data->type() == Number::TYPE)
			{
				lua_pushnumber(lua, data->number());
				return true;
			}
			if (data->type() == Boolean::TYPE)
			{
				lua_pushboolean(lua, data->boolean());
				return true;
			}
			if (data->type() == String::TYPE)
			{
				lua_pushstring(lua, data->string());
				return true;
			}
		}
		return false;
	}

}
}
}
}
