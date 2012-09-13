#include "lua_state.h"

extern "C" 
{
#	include <lua/src/lstate.h>
}

#include <sstream>

#include <log/logger.h>

namespace am {
namespace lua {

	LuaState::LuaState()
	{
		mLua = luaL_newstate(); 
		luaL_openlibs(mLua);
		lua_pushcfunction(mLua, onError);
	}
	LuaState::~LuaState()
	{
		lua_close(mLua);
	}

	lua_State *LuaState::getLua()
	{
		return mLua;
	}

	LuaState::operator lua_State *()
	{
		return mLua;
	}

	int LuaState::onError(lua_State *lua)
	{
		return 0;
	}

	bool LuaState::loadFile(const char *filename)
	{
		return luaL_dofile(mLua, filename);
	}
	bool LuaState::loadString(const char *luaString)
	{
		return luaL_dostring(mLua, luaString);
	}

	int LuaState::newTable(const char *tableName)
	{
		lua_newtable(mLua);
		lua_pushvalue(mLua, -1);
		if (tableName != NULL)
		{
			lua_pushvalue(mLua, -1);
			lua_setglobal(mLua, tableName);
		}
		int ref = luaL_ref(mLua, LUA_REGISTRYINDEX);
		return ref;
	}
	bool LuaState::getTable(int tableRef)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, tableRef);
		if (lua_istable(mLua, -1))
		{
			return true;
		}
		lua_pop(mLua, -1);
		return false;
	}
	bool LuaState::getTable(const char *tableName)
	{
		lua_getglobal(mLua, tableName);
		if (lua_istable(mLua, -1))
		{
			return true;
		}
		lua_pop(mLua, -1);
		return false;
	}

	bool LuaState::compareRefs(int ref1, int ref2)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, ref1);
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, ref2);
		bool result = lua_rawequal(mLua, -1, -2);
		lua_pop(mLua, -2);
		return result;
	}

	void LuaState::pop(int num)
	{
		lua_pop(mLua, num);
	}
	void LuaState::push(const char *str)
	{
		lua_pushstring(mLua, str);
	}
	void LuaState::push(int num)
	{
		lua_pushinteger(mLua, num);
	}
	void LuaState::push(double num)
	{
		lua_pushnumber(mLua, num);
	}
	void LuaState::push(bool val)
	{
		lua_pushboolean(mLua, val);
	}
	void LuaState::pushnil()
	{
		lua_pushnil(mLua);
	}

	void LuaState::setTableValue(const char *key, const char *value)
	{
		lua_pushstring(mLua, key);
		lua_pushstring(mLua, value);
		lua_settable(mLua, -3);
	}
	void LuaState::setTableValue(const char *key, int value)
	{
		lua_pushstring(mLua, key);
		lua_pushinteger(mLua, value);
		lua_settable(mLua, -3);
	}
	void LuaState::setTableValue(const char *key, double value)
	{
		lua_pushstring(mLua, key);
		lua_pushnumber(mLua, value);
		lua_settable(mLua, -3);
	}
	void LuaState::setTableValue(const char *key, bool value)
	{
		lua_pushstring(mLua, key);
		lua_pushboolean(mLua, value);
		lua_settable(mLua, -3);
	}

	const char *LuaState::getTableString(const char *key)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		const char *result = lua_tostring(mLua, -1);
		lua_pop(mLua, 1);
		return result;
	}
	bool LuaState::getTableInt(const char *key, int &value)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		if (lua_isnumber(mLua, -1))
		{
			value = static_cast<int>(lua_tointeger(mLua, -1));
			lua_pop(mLua, 1);
			return true;
		}
		lua_pop(mLua, 1);
		return false;
	}
	bool LuaState::getTableDouble(const char *key, double &value)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		if (lua_isnumber(mLua, -1))
		{
			value = static_cast<double>(lua_tonumber(mLua, -1));
			lua_pop(mLua, 1);
			return true;
		}
		lua_pop(mLua, 1);
		return false;
	}
	bool LuaState::getTableBool(const char *key, bool &value)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		if (lua_isboolean(mLua, -1))
		{
			value = lua_toboolean(mLua, -1) > 0;
			lua_pop(mLua, 1);
			return true;
		}
		lua_pop(mLua, 1);
		return false;
	}

	bool LuaState::hasGlobalFunction(const char *func, bool popAfter)
	{
		lua_getglobal(mLua, func);
		bool isFunc = lua_isfunction(mLua, -1);
		if (popAfter && isFunc || !isFunc)
		{
			lua_pop(mLua, 1);
		}
		return isFunc;
	}

	void LuaState::logStack(const char *cat)
	{
		stringstream ss;
		printStack(ss);
		am_log(cat, ss);
	}

	void LuaState::printStack(ostream &output)
	{
		bool done = false;
		int size = lua_gettop(mLua);
		for (int i = 1; i <= size && !done; i++)
		{
			output << i << ": ";
			
			int type = lua_type(mLua, i);
			switch(type) 
			{
			case LUA_TTABLE:
				output << "Table\n";
				break;
			case LUA_TFUNCTION:
				output << "Function\n";
				break;
			case LUA_TNIL:
				output << "Nil\n";
				break;
			case LUA_TBOOLEAN:
				output << "Boolean | " << (lua_toboolean(mLua, i) ? "true\n" : "false\n");
				break;
			case LUA_TLIGHTUSERDATA:
				output << "Light user data\n";
				break;
			case LUA_TTHREAD:
				output << "Thread\n";
				break;
			case LUA_TSTRING:
				output << "String | " << lua_tostring(mLua, i) << "\n";
				break;
			default:
				if (lua_isnumber(mLua, i))
				{
					output << "Number | " << lua_tonumber(mLua, i) << "\n";
				}
				else
				{
					output << "Unknown!\n";
					done = true;
				}
			}
		}
	}

}
}
