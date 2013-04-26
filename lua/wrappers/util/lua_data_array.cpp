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
	 * Compares this Array with another Array object.
	 *
	 * @param Array rhs The other Array to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int DataArray_eq(lua_State *lua)
	{
		Array *lhs = castUData<Array>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "Array");
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
	 * Returns the asset name currently used by this Array.
	 * An empty string indicates that no asset has been set.
	 *
	 * @returns string The asset name is use.
	 */
	/**
	 * Sets the asset name for this Array to use.
	 *
	 * @param string assetName The name of the asset to use.
	 * @returns Array This
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
					if (data)
					{
						if (data->type() == Array::TYPE)
						{
							wrapRefObject<Array>(lua, dynamic_cast<Array *>(data));
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
		return LuaState::expectedContext(lua, "at", "Array");
	}

	int DataArray_push(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			int luaType = lua_type(lua, 2);
			if (luaType == LUA_TNUMBER || luaType == LUA_TBOOLEAN || 
				luaType == LUA_TSTRING || luaType == LUA_TTABLE ||
				luaType == LUA_TNIL)
			{
				LuaState L(lua);
				IData *data = IData::fromLua(L, 2);
				if (data)
				{
					arr->push(data);
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "push", 2, "primative value");
		}
		return LuaState::expectedContext(lua, "push", "Array");
	}

	int DataArray_remove(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			if (lua_isnum(lua, 2))
			{
				arr->remove(lua_tointeger(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "remove", "integer index");
		}
		return LuaState::expectedContext(lua, "remove", "Array");
	}

	int DataArray_len(lua_State *lua)
	{
		Array *arr = castUData<Array>(lua, 1);
		if (arr)
		{
			lua_pushnumber(lua, arr->inner().size());
		}
		return LuaState::expectedContext(lua, "__len", "Array");
	}
}
}
}
}
