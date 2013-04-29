#include "lua_data_array.h"

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

#include "lua_data_map.h"

namespace am {
namespace lua {
namespace util {
namespace data {
	/**
	 * @class
	 * Simple wrapper for the util::data::Array class. Allows for storing
	 * and retrieving tables of primative data, including other tables, 
	 * arrays, numbers, booleans and strings.
	 */
	/**
	 * Creates a new util::data::Array with no data.
	 */
	int DataArray_ctor(lua_State *lua)
	{
		Array *arr = new Array();
		wrapRefObject<Array>(lua, arr);
		return 1;
	}
	/**
	 * Releases the reference counter on the util::data::Array.
	 */
	int DataArray_dtor(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			arr->release();
		}
		return 0;
	}
	/**
	 * Compares this data array with another data array object.
	 *
	 * @param DataArray rhs The other data array to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int DataArray_eq(lua_State *lua)
	{
		Array *lhs = castUData<Array>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "DataArray");
		}
		Array *rhs = castUData<Array>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int DataArray_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", DataArray_ctor },
			{ "__gc", DataArray_dtor },
			{ "__eq", DataArray_eq },
			{ "__len", DataArray_len },
			{ "at", DataArray_at },
			{ "push", DataArray_push },
			{ "remove", DataArray_remove },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Array::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the value at the given index.
	 * 
	 * A primative is a number, string, boolean, nil or table of primatives.
	 * 
	 * @param integer index The index to look up.
	 * @returns primative The value.
	 */
	/**
	 * Sets the value at the given index.
	 * 
	 * A primative is a number, string, boolean, nil or table of primatives.
	 *
	 * @param integer index The index to set.
	 * @param primative value The value to set.
	 * @returns DataArray This
	 */
	int DataArray_at(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			if (lua_isnum(lua, 2))
			{
				if (lua_gettop(lua) == 2)
				{
					IData *data = arr->at(lua_tointeger(lua, 2));
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
							arr->inner()[lua_tointeger(lua, 2)] = data;
						}
						else
						{
							arr->remove(lua_tointeger(lua, 2));
						}
						lua_first(lua);
					}
				}
			}
			return LuaState::expectedArgs(lua, "at", 2, "string key", "string key, primative value");
		}
		return LuaState::expectedContext(lua, "at", "DataArray");
	}

	/**
	 * Pushes a value or list of primatives on to the end of this data array.
	 * 
	 * A primative is a number, string, boolean, nil or table of primatives.
	 *
	 * <pre>
	 * DataArray = import("DataArray")
	 * array = DataArray.new()
	 * array:push(5)
	 * am_log("Size 1: " .. #array)		-- Outputs: Size 1: 1
	 * 
	 * array:push("String", true)
	 * am_log("Size 2: " .. #array)		-- Outputs: Size 2: 3
	 * </pre>
	 * 
	 * @param primative... values The list of values to push on to the end.
	 * @returns DataArray This
	 */
	int DataArray_push(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			bool valid = true;
			int args = lua_gettop(lua);
			if (args > 1)
			{
				for (int n = 2; n < args; ++n)
				{
					int luaType = lua_type(lua, n);
					if (!IS_LUA_PRIMATIVE(luaType))
					{
						valid = false;
						break;
					}
				}
				if (valid)
				{
					LuaState L(lua);
					for (int n = 2; n < args; ++n)
					{
						IData *data = IData::fromLua(L, n);
						if (data)
						{
							arr->push(data);
						}
					}
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "push", 2, "primative value");
		}
		return LuaState::expectedContext(lua, "push", "DataArray");
	}

	/**
	 * Removes the indicies from the given list of integers.
	 * This takes a variable number of arguments which will be validated
	 * as being integers before an index is removed.
	 * It is not considered an array to remove an index that is out of bounds.
	 *
	 * <pre>
	 * DataArray = import("DataArray")
	 * array:push(5, "String", true)
	 * am_log("Size 1: " .. #array)		-- Outputs: Size 1: 3
	 *
	 * array:remove(2, 0, 1)			-- Attempts to remove index 2, 0 then 1.
	 *									-- At the end index 1 is out of bounds and is ignored.
	 * am_log("Size 2: " .. #array)		-- Outputs: Size 2: 1
	 * </pre>
	 *
	 * @param integer.. indicies The indicies to remove.
	 * @returns DataArray This
	 */
	int DataArray_remove(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			int n = 2;
			int args = lua_gettop(lua);
			if (args > 1)
			{
				bool valid = true;
				for (n = 2; n < args; ++n)
				{
					if (!lua_isnum(lua, n))
					{
						valid = false;
						break;
					}
				}

				if (valid)
				{
					for (n = 2; n < args; n++)
					{
						arr->remove(lua_tointeger(lua, 2));
					}
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "remove", "integer index ...");
		}
		return LuaState::expectedContext(lua, "remove", "DataArray");
	}

	/**
	 * Returns the length of this data array.
	 * Is used by the Lua # operator.
	 *
	 * @returns integer The length of this data array.
	 */
	int DataArray_len(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			lua_pushnumber(lua, arr->inner().size());
			return 1;
		}
		return LuaState::expectedContext(lua, "__len", "DataArray");
	}
}
}
}
}
