#include "lua_state.h"

#include <sstream>

#include <log/logger.h>

#include <lua/wrappers/lua_wrappers.h>

namespace am {
namespace lua {

	LuaState::WrapperMap LuaState::sWrapperMap;

	LuaState::LuaState(bool includeLibraries)
	{
		mLua = luaL_newstate(); 
		if (includeLibraries)
		{
			luaL_openlibs(mLua);
			lua_register(mLua, "import", getWrapper);
		}
		// We always want error handler and logger function.
		lua_atpanic(mLua, onError);
		lua_register(mLua, "am_log", lua_am_log);

		am::lua::wrapper::AssignWrappers(mLua);
	}
	LuaState::LuaState(lua_State *lua)
	{
		mLua = lua;
	}
	LuaState::~LuaState()
	{
		
	}

	lua_State *LuaState::getLua()
	{
		return mLua;
	}
	void LuaState::close()
	{
		lua_close(mLua);
	}

	LuaState::operator lua_State *()
	{
		return mLua;
	}

	int LuaState::onError(lua_State *lua)
	{
		stringstream ss;
		printStack(lua, ss);
		const char *sss = ss.str().c_str();
		return 0;
	}

	bool LuaState::loadFile(const char *filename)
	{
		return !luaL_dofile(mLua, filename);
	}
	bool LuaState::loadString(const char *luaString)
	{
		return !luaL_dostring(mLua, luaString);
	}

	void LuaState::call(int n, int r)
	{
		lua_call(mLua, n, r);
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
		logStack("LOAD");
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

	bool LuaState::compareRefs(int ref1, int ref2) const
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, ref1);
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, ref2);
		bool result = lua_rawequal(mLua, -1, -2) > 0;
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

	bool LuaState::isTableNumber(const char *key)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		if (lua_isnumber(mLua, -1))
		{
			return true;
		}
		lua_pop(mLua, 1);
		return false;
	}
	bool LuaState::isTableString(const char *key)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		if (lua_isstring(mLua, -1))
		{
			return true;
		}
		lua_pop(mLua, 1);
		return false;
	}
	bool LuaState::isTableTable(const char *key)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		if (lua_istable(mLua, -1))
		{
			return true;
		}
		lua_pop(mLua, 1);
		return false;
	}
	bool LuaState::isTableBool(const char *key)
	{
		lua_pushstring(mLua, key);
		lua_gettable(mLua, -2);
		if (lua_isboolean(mLua, -1))
		{
			return true;
		}
		lua_pop(mLua, 1);
		return false;
	}

	int LuaState::hasTableValue(const char *key)
	{
		lua_pushstring(mLua, key);
		return lua_type(mLua, -1);
	}

	float LuaState::toNumber(int arg)
	{
		float val = static_cast<float>(lua_tonumber(mLua, arg));
		lua_pop(mLua, 1);
		return val;
	}
	int LuaState::toInteger(int arg)
	{
		int val = lua_tointeger(mLua, arg);
		lua_pop(mLua, 1);
		return val;
	}
	bool LuaState::toBool(int arg)
	{
		bool val = lua_toboolean(mLua, arg);
		lua_pop(mLua, 1);
		return val;
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

	int LuaState::getGlobalInt(const char *name)
	{
		lua_getglobal(mLua, name);
		int value = lua_tointeger(mLua, -1);
		lua_pop(mLua, 1);
		return value;
	}
	double LuaState::getGlobalDouble(const char *name)
	{
		lua_getglobal(mLua, name);
		double value = lua_tonumber(mLua, -1);
		lua_pop(mLua, 1);
		return value;
	}
	bool LuaState::getGlobalBool(const char *name)
	{
		lua_getglobal(mLua, name);
		bool value = lua_toboolean(mLua, -1) > 0;
		lua_pop(mLua, 1);
		return value;
	}
	string LuaState::getGlobalString(const char *name)
	{
		lua_getglobal(mLua, name);
		const char *value = lua_tostring(mLua, -1);
		string valueStr;
		if (value != NULL)
		{
			valueStr = value;
		}
		lua_pop(mLua, 1);
		return valueStr;
	}

	void LuaState::logStack(const char *cat)
	{
		stringstream ss;
		printStack(mLua, ss);
		am_log(cat, ss);
	}
	void LuaState::logStack(lua_State *lua, const char *cat)
	{
		stringstream ss;
		printStack(lua, ss);
		am_log(cat, ss);
	}

	void LuaState::printStack(ostream &output)
	{
		printStack(mLua, output);
	}
	void LuaState::printStack(lua_State *lua, ostream &output)
	{
		bool done = false;
		int size = lua_gettop(lua);
		for (int i = 1; i <= size && !done; i++)
		{
			output << i << ": ";
			
			int type = lua_type(lua, i);
			output << ttypename(type);
			switch(type) 
			{
			case LUA_TBOOLEAN:
				output << ' ' << (lua_toboolean(lua, i) ? "true\n" : "false\n");
				break;
			case LUA_TSTRING:
				output << ' ' << lua_tostring(lua, i) << '\n';
				break;
			default:
				if (lua_isnumber(lua, i))
				{
					output << ' ' << lua_tonumber(lua, i) << '\n';
				}
				else
				{
					output << '\n';
				}
			}
		}
	}

	bool LuaState::operator==(const lua_State *lua) const
	{
		return mLua == lua;
	}
	bool LuaState::operator==(const LuaState &lua) const
	{
		return mLua == lua.mLua;
	}

	bool LuaState::operator!=(const lua_State *lua) const
	{
		return mLua != lua;
	}
	bool LuaState::operator!=(const LuaState &lua) const
	{
		return mLua != lua.mLua;
	}

	void LuaState::registerWrapper(const char *name, lua_CFunction call)
	{
		sWrapperMap[string(name)] = call;
	}
	int LuaState::getWrapper(lua_State *lua)
	{
		if (lua_isstring(lua, -1))
		{
			string name = lua_tostring(lua, -1);
			WrapperMap::iterator iter = sWrapperMap.find(name);
			if (iter != sWrapperMap.end())
			{
				int result = iter->second(lua);
				return result;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

	int LuaState::lua_am_log(lua_State *lua)
	{
		if (lua_isstring(lua, -1) || lua_isnumber(lua, -1))
		{
			am_log("LUA", lua_tostring(lua, -1));
		}
		else if (lua_isboolean(lua, -1))
		{
			bool value = lua_toboolean(lua, -1);
			am_log("LUA", value ? "true" : "false");
		}
		return 0;
	}

}
}
