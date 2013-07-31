#include "lua_data_table.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/data_nil.h>
using namespace am::util;

namespace am {
namespace lua {
namespace util {
namespace data {
	/**
	 * @class
	 * Simple wrapper for the am::util::data::Table class. Allows for storing
	 * and retrieving tables of primative data, including other tables, numbers, booleans and strings. <br>
	 * 
	 * While values like numbers, strings, booleans and nil remain essentially
	 * unchanged when they are added to a data table, tables do change.
	 * Tables are turned into DataTables, which being a custom class means that normal
	 * table functions will not work. All values should remain the same however.
	 */
	/**
	 * Creates a new DataTable with no data.
	 */
	int DataTable_ctor(lua_State *lua)
	{
		am::util::data::Table *table = new am::util::data::Table();
		wrapRefObject<am::util::data::Table>(lua, table);
		return 1;
	}
	/**
	 * Releases the reference counter on the DataTable.
	 */
	int DataTable_dtor(lua_State *lua)
	{
		am::util::data::Table *table = castUData<am::util::data::Table>(lua, 1);
		if (table)
		{
			table->release();
		}
		return 0;
	}
	/**
	 * Compares this data table with another data table object.
	 *
	 * @param DataTable rhs The other data table to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int DataTable_eq(lua_State *lua)
	{
		am::util::data::Table *lhs = castUData<am::util::data::Table>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "DataTable");
		}
		am::util::data::Table *rhs = castUData<am::util::data::Table>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int DataTable_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", DataTable_ctor },
			{ "__gc", DataTable_dtor },
			{ "__eq", DataTable_eq },
			{ "__len", DataTable_len },

			{ "at", DataTable_at },
			{ "push", DataTable_push },
			{ "pairs", DataTable_pairs },
			{ "ipairs", DataTable_ipairs },
			{ "remove", DataTable_remove },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, am::util::data::Table::LUA_TABLENAME);
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
	 * @returns DataTable This
	 */
	int DataTable_at(lua_State *lua)
	{
		am::util::data::Table *table = castUData<am::util::data::Table>(lua, 1);
		if (table)
		{
			if (lua_isstr(lua, 2))
			{
				if (lua_gettop(lua) == 2)
				{
					am::util::data::IData *data = table->at(lua_tostring(lua, 2));
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
						am::util::data::IData *data = am::util::data::IData::fromLua(L, 3);
						if (data)
						{
							table->at(lua_tostring(lua, 2), data);
							lua_first(lua);
						}
					}
				}
			}
			else if (lua_isnum(lua, 2))
			{
				if (lua_gettop(lua) == 2)
				{
					am::util::data::IData *data = table->at(lua_tointeger(lua, 2) - 1);
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
						am::util::data::IData *data = am::util::data::IData::fromLua(L, 3);
						if (data)
						{
							table->at(lua_tointeger(lua, 2) - 1, data);
							lua_first(lua);
						}
					}
				}
			}
			return LuaState::expectedArgs(lua, "at", 4, "integer index", "integer index, primative value", "string key", "string key, primative value");
		}
		return LuaState::expectedContext(lua, "at", "DataTable");
	}

	/**
	 * Pushes a value or list of primatives on to the end of this data table.
	 * 
	 * A primative is a number, string, boolean, nil or table of primatives.
	 *
	 * <pre>
	 * DataTable = import("DataTable")
	 * array = DataTable.new()
	 * array:push(5)
	 * am_log("Size 1: " .. #array)		-- Outputs: Size 1: 1
	 * 
	 * array:push("String", true)
	 * am_log("Size 2: " .. #array)		-- Outputs: Size 2: 3
	 * </pre>
	 * 
	 * @param primative... values The list of values to push on to the end.
	 * @returns DataTable This
	 */
	int DataTable_push(lua_State *lua)
	{
		am::util::data::Table *table = castUData<am::util::data::Table>(lua, 1);
		if (table)
		{
			bool valid = true;
			int args = lua_gettop(lua);
			if (args > 1)
			{
				for (int n = 2; n <= args; ++n)
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
					for (int n = 2; n <= args; ++n)
					{
						am::util::data::IData *data = am::util::data::IData::fromLua(L, n);
						if (data)
						{
							table->push(data);
						}
					}
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "push", 2, "primative value");
		}
		return LuaState::expectedContext(lua, "push", "DataTable");
	}

	/**
	 * Returns the number of key values in the data table.
	 * This is an implementation of the __len metamethod as such the
	 * Lua # operator can be used to get the size.
	 *
	 * @returns integer The number of key values in the data table.
	 */
	int DataTable_len(lua_State *lua)
	{
		am::util::data::Table *table = castUData<am::util::data::Table>(lua, 1);
		if (table)
		{
			lua_pushinteger(lua, table->mapInner().size() + table->arrayInner().size());
			return 1;
		}
		return LuaState::expectedContext(lua, "__len", "DataTable");
	}

	/**
	 * Removes the keys given list of strings.
	 * This takes a variable number of arguments which will be validated
	 * as being strings before a key is removed.
	 * It is not considered an error to remove a key that does not exist.
	 *
	 * <pre>
	 * DataTable = import("DataTable")
	 * table = DataTable.new()
	 * table:at("num", 5)
	 * table:at("str", "String")
	 * table:at("bool", true)
	 * 
	 * am_log("Size 1: " .. #table)	-- Outputs: Size 1: 3
	 * table:remove("num", "bool")
	 * 
	 * am_log("Size 2: " .. #table)	-- Outputs: Size 2: 1
	 * </pre>
	 * 
	 * @param string... keys The keys to remove.
	 * @returns DataTable This
	 */
	int DataTable_remove(lua_State *lua)
	{
		am::util::data::Table *table = castUData<am::util::data::Table>(lua, 1);
		if (table)
		{
			int args = lua_gettop(lua);
			if (args > 1)
			{
				bool valid = true;
				// Make sure that all the arguments are strings before removing anything.
				for (int n = 2; n < args; ++n)
				{
					if (!lua_isstr(lua, n) && !lua_isnum(lua, n))
					{
						valid = false;
						break;
					}
				}
				if (valid)
				{
					for (int n = 2; n < args; ++n)
					{
						if (lua_isstr(lua, n))
						{
							table->remove(lua_tostring(lua, n));
						}
						else
						{
							table->remove(lua_tointeger(lua, n));
						}
					}
					lua_first(lua);
				}
			}
			return LuaState::expectedArgs(lua, "remove", 2, "string/integer key/index ...");
		}
		return LuaState::expectedContext(lua, "remove", "DataTable");
	}

	/**
	 * The pairs function provides similar functionality to that of the Lua pairs function.
	 * The pairs function takes a function as it's argument and that function will be
	 * called once per key/value in the data table. If that function returns false
	 * then the loop is stopped.
	 *
	 * <pre>
	 * DataTable = import("DataTable")
	 * table = DataTable.new()
	 * table:at("num", 5)
	 * table:at("str", "String")
	 * table:at("bool", true)
	 * table:at(1, "one")
	 * table:at(2, "two")
	 * table:pairs(function(key, value)\n
	 *     am_log(key .. " = ", value)
	 * end)
	 * -- The output order is not gurrantied
	 * -- 1 = "one"
	 * -- 2 = "two"
	 * -- "num" = 5
	 * -- "str" = "String"
	 * -- "bool" = true
	 * </pre>
	 *
	 * @param function handler The each function handler.
	 * @returns DataTable This
	 */
	int DataTable_pairs(lua_State *lua)
	{
		am::util::data::Table *table = castUData<am::util::data::Table>(lua, 1);
		if (table)
		{
			if (lua_isfunction(lua, 2))
			{
				bool finished = false;
				int i = 1;
				for (auto iter = table->beginArray(); iter != table->endArray(); ++iter, ++i)
				{
					lua_pushvalue(lua, 2);
					lua_pushinteger(lua, i);
					if (!pushData(lua, iter->get()))
					{
						lua_pushnil(lua);
					}
					lua_call(lua, 2, 1);
					if (lua_isbool(lua, -1) && !lua_tobool(lua, -1))
					{
						finished = true;
						break;
					}
				}
				if (finished)
				{
					lua_first(lua);
				}
				for (auto iter = table->beginMap(); iter != table->endMap(); ++iter)
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
			return LuaState::expectedArgs(lua, "pairs", "function handler");
		}
		return LuaState::expectedContext(lua, "pairs", "DataTable");
	}

	/**
	 * The ipairs function provides similar functionality to that of the Lua ipairs function.
	 * The ipairs function takes a function as it's argument and that function will be
	 * called once per index/value in the data table. If that function returns false
	 * then the loop is stopped.
	 *
	 * <pre>
	 * DataTable = import("DataTable")
	 * table = DataTable.new()
	 * table:push("one", "two", 3)
	 * table:ipairs(function(index, value)\n
	 *     am_log(index .. " = ", value)
	 * end)
	 * -- The output order is not gurrantied
	 * -- 1 = "one"
	 * -- 2 = "two"
	 * -- 3 = 3
	 * </pre>
	 *
	 * @param function handler The each function handler.
	 * @returns DataTable This
	 */
	int DataTable_ipairs(lua_State *lua)
	{
		am::util::data::Table *table = castUData<am::util::data::Table>(lua, 1);
		if (table)
		{
			if (lua_isfunction(lua, 2))
			{
				int i = 1;
				for (auto iter = table->beginArray(); iter != table->endArray(); ++iter, ++i)
				{
					lua_pushvalue(lua, 2);
					lua_pushinteger(lua, i);
					if (!pushData(lua, iter->get()))
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
			return LuaState::expectedArgs(lua, "ipairs", "function handler");
		}
		return LuaState::expectedContext(lua, "ipairs", "DataTable");
	}

	bool pushData(lua_State *lua, am::util::data::IData *data)
	{
		if (data)
		{
			if (data->type() == am::util::data::Table::TYPE)
			{
				wrapRefObject<am::util::data::Table>(lua, dynamic_cast<am::util::data::Table *>(data));
				return true;
			}
			if (data->type() == am::util::data::Number::TYPE)
			{
				lua_pushnumber(lua, data->number());
				return true;
			}
			if (data->type() == am::util::data::Boolean::TYPE)
			{
				lua_pushboolean(lua, data->boolean());
				return true;
			}
			if (data->type() == am::util::data::String::TYPE)
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
