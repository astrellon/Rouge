#include "lua_state.h"

#include <sstream>

#include <log/logger.h>

#include <lua/wrappers/lua_wrappers.h>

namespace am {
namespace lua {

	jmp_buf LuaState::sRecoverBuff;

	int getUDataType(lua_State *lua, int n)
	{
		if (!lua_isuserdata(lua, n))
		{
			return -1;
		}
		int *data = reinterpret_cast<int *>(lua_touserdata(lua, n));
		return data[0];
	}

	LuaState::WrapperMap LuaState::sWrapperMap;
	LuaState::WrapperIdMap LuaState::sWrapperIdMap;
	int LuaState::sWrapperMaxId = -1;
	int LuaState::sDepth = 0;

	LuaState::LuaState(bool includeLibraries)
	{
		mLua = luaL_newstate();
		// We always want the standard libs as they provide basic table manipulation.
		luaL_openlibs(mLua);
		if (includeLibraries)
		{
			lua_register(mLua, "import", getWrapper);
			lua_register(mLua, "am_equals", luaEquals);
			lua_register(mLua, "not_equals", luaNotEquals);
			lua_register(mLua, "print_stack", luaPrintStack);
		}
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
		{
			stringstream ss;
			printStack(lua, ss);
			am_log("LUAERR", ss);
		}
		return 0;
	}

	/**
	 * Attemps to load the given file as Lua code and returns true if this succeeds.
	 * If there is an error, use either logStack or printStack to get the result.
	 */
	bool LuaState::loadFile(const char *filename)
	{
		return !luaL_dofile(mLua, filename);
	}
	/**
	 * Attemps to load the given string as Lua code and returns true if this succeeds.
	 * If there is an error, use either logStack or printStack to get the result.
	 */
	bool LuaState::loadString(const char *luaString)
	{
		return !luaL_dostring(mLua, luaString);
	}
	
	void LuaState::call(int n, int r)
	{
		int result = lua_pcall(mLua, n, r, 0);
		if (result != LUA_OK)
		{
			stringstream errss;
			errss << "Error calling Lua: " << result << '\n';
			printStack(mLua, errss);
			throw std::runtime_error(errss.str().c_str());
		}
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
		if (lua_type(mLua, -1) == LUA_TNUMBER)
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
		if (lua_type(mLua, -1) == LUA_TNUMBER)
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
		if (lua_type(mLua, -1) == LUA_TBOOLEAN)
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
		if (lua_type(mLua, -1) == LUA_TNUMBER)
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
		if (lua_type(mLua, -1) == LUA_TSTRING)
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
		if (lua_type(mLua, -1) == LUA_TTABLE)
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
	/**
	 * Calls lua_tonumber on top of the stack and pops the stacks.
	 */
	float LuaState::toNumber()
	{
		float val = static_cast<float>(lua_tonumber(mLua, -1));
		lua_pop(mLua, 1);
		return val;
	}
	/**
	 * Calls lua_tointeger on top of the stack and pops the stacks.
	 */
	int LuaState::toInteger()
	{
		int val = lua_tointeger(mLua, -1);
		lua_pop(mLua, 1);
		return val;
	}
	/**
	 * Calls lua_toboolean on top of the stack and pops the stacks.
	 */
	bool LuaState::toBool()
	{
		bool val = lua_tobool(mLua, -1);
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
	bool LuaState::getGlobal(const char *name)
	{
		lua_getglobal(mLua, name);
		if (lua_isnil(mLua, -1))
		{
			pop(1);
			return false;
		}
		return true;
	}

	void LuaState::logTable(const char *cat, int n)
	{
		stringstream ss;
		printTable(ss, n);
		am_log(cat, ss);
	}
	void LuaState::printTable(ostream &output, int n, bool includeType)
	{
		if (!lua_istable(mLua, n))
		{
			printTypeValue(mLua, n, output, includeType);
			output << '\n';
		}
		else
		{
			sDepth++;
			output << "{\n";
			if (sDepth > 10) {
				output << "<Reached " << sDepth << " deep>";
			}
			else {
				lua_pushnil(mLua);
				while (lua_next(mLua, -2) != 0)
				{
					for (int i = 0; i < sDepth; i++)
					{
						output << '\t';
					}
					if (lua_type(mLua, -2) == LUA_TSTRING)
					{
						output << lua_tostring(mLua, -2);
					}
					else
					{
						output << lua_tonumber(mLua, -2);
					}
					output << " = ";
					printTable(output, -1, includeType);
					pop(1);
				}
			}
			sDepth--;
			for (int i = 0; i < sDepth; i++)
			{
				output << '\t';
			}
			output << "}\n";
		}
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

	int LuaState::luaPrintStack(lua_State *lua)
	{
		stringstream ss;
		printStack(lua, ss);
		lua_pushstring(lua, ss.str().c_str());
		return 1;
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
			
			printTypeValue(lua, i, output);

			output << '\n';
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

	void LuaState::registerWrapper(const char *name, lua_CFunction call, int id)
	{
		sWrapperMap[string(name)] = call;
		sWrapperIdMap[id] = string(name);
		if (id > sWrapperMaxId)
		{
			sWrapperMaxId = id;
		}
	}
	int LuaState::getMaxRegisteredId()
	{
		return sWrapperMaxId;
	}
	int LuaState::getWrapper(lua_State *lua)
	{
		int args = lua_gettop(lua);
		for (int i = 1; i <= args; i++)
		{
			if (lua_isstr(lua, i))
			{
				string name = lua_tostring(lua, i);
				WrapperMap::iterator iter = sWrapperMap.find(name);
				if (iter != sWrapperMap.end())
				{
					iter->second(lua);
				}
				continue;
			}
			lua_pushnil(lua);
		}
		return args;
	}
	void LuaState::clearRegistered()
	{
		sWrapperMap.clear();
	}

	int LuaState::expectedArgs(lua_State *lua, const char *funcName, const char *expected)
	{
		if (funcName == NULL || funcName[0] == '\0')
		{
			return 0;
		}
		stringstream ss;
		int args = lua_gettop(lua);
		int start = 2;
		if (funcName[0] == '@')
		{
			start = 1;
			funcName = funcName + 1;
		}
		for (int i = start; i <= args; i++)
		{
			if (i > start) {
				ss << ", ";
			}
			printTypeValue(lua, i, ss, true);
		}
		luaL_error(lua, "Expected args: %s(%s) got %s(%s)", funcName, expected, funcName, ss.str().c_str());
		return 0;
	}
	int LuaState::expectedArgs(lua_State *lua, const char *funcName, int n, ...)
	{
		if (funcName == NULL || funcName[0] == '\0')
		{
			return 0;
		}

		int start = 2;
		if (funcName[0] == '@')
		{
			start = 1;
			funcName = funcName + 1;
		}

		stringstream gotss;
		int args = lua_gettop(lua);
		for (int i = start; i <= args; i++)
		{
			if (i > start) 
			{
				gotss << ", ";
			}
			printTypeValue(lua, i, gotss, true);
		}

		stringstream ss;
		LuaState::printStack(lua, ss);

		stringstream expss;
		va_list list;
		va_start(list, n);
		for (int i = 0; i < n; i++)
		{
			if (i > 0)
			{
				expss << " or ";
			}
			expss << '(' << va_arg(list, const char *) << ')';
		}
		luaL_error(lua, "Expected %s%s got %s(%s)", funcName, expss.str().c_str(), funcName, gotss.str().c_str());
		return 0;
	}
	int LuaState::expectedContext(lua_State *lua, const char *funcName, const char *expected)
	{
		luaL_error(lua, "Wrong context, expected %s.%s got %s.%s", expected, funcName, getType(lua, 1), funcName);
		return 0;
	}

	void LuaState::warning(lua_State *lua, const char *message)
	{
		lua_Debug ar;
		lua_getstack(lua, 1, &ar);
		lua_getinfo(lua, "nSl", &ar);
		int line = ar.currentline;
		stringstream ss;
		ss << message << " [" << line << ']';
		am_log("LUA WARN", ss);
	}

	int LuaState::luaEquals(lua_State *lua)
	{
		if (lua_compare(lua, 1, 2, LUA_OPEQ) != 1)
		{
			stringstream ss;
			ss << "\n- Expected:\t";
			printTypeValue(lua, 1, ss);
			ss << "\n- Actual:\t\t";
			printTypeValue(lua, 2, ss);
			return luaL_error(lua, "%s", ss.str().c_str());
		}
		return 0;
	}
	int LuaState::luaNotEquals(lua_State *lua)
	{
		if (lua_compare(lua, 1, 2, LUA_OPEQ) == 1)
		{
			stringstream ss;
			ss << "\n- Did not expect:\t";
			printTypeValue(lua, 1, ss);
			ss << "\n- Actual:\t\t";
			printTypeValue(lua, 2, ss);
			return luaL_error(lua, "%s", ss.str().c_str());
		}
		return 0;
	}
	void LuaState::printTypeValue(lua_State *lua, int n, ostream &output, bool includeType)
	{
		int type = lua_type(lua, n);
		if (includeType)
		{
			output << ttypename(type) << ' ';
		}
		switch(type) 
		{
		case LUA_TNIL:
			output << "nil";
			break;
		case LUA_TBOOLEAN:
			output << (lua_tobool(lua, n) ? "true" : "false");
			break;
		case LUA_TSTRING:
			output << lua_tostring(lua, n);
			break;
		case LUA_TNUMBER:
			output << lua_tonumber(lua, n);
			break;
		case LUA_TTABLE:
			output << "Table";
			break;
		case LUA_TFUNCTION:
			output << "Function";
			break;
		case LUA_TUSERDATA:
			{
				output << getType(lua, n);
			}
			break;
		default:
			output << "Unknown";
		}
	}

	const char *LuaState::getType(lua_State *lua, int n)
	{
		int type = lua_type(lua, n);
		if (type == LUA_TUSERDATA)
		{
			LuaUData *udata = reinterpret_cast<LuaUData *>(lua_touserdata(lua, n));
			if (sWrapperIdMap.find(udata->id) != sWrapperIdMap.end())
			{
				return sWrapperIdMap[udata->id].c_str();
			}
			else 
			{
				return "Unknown userdata";
			}
		}
		return lua_typename(lua, n);
	}

	int LuaState::lua_am_log(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 0)
		{
			return 0;
		}
		stringstream ss;
		LuaState L(lua);
		for (int i = 1; i <= args; i++)
		{
			if (i > 1)
			{
				ss << ", ";
			}
			L.printTable(ss, i, false);
		}
		am_log("LUA", ss);
		
		return 0;
	}

	void LuaState::displayLineError(lua_State *lua, const char *file, int line)
	{
		stringstream ss;
		ss << "Error calling Lua in " << file << " [" << line << "]\n";
		printStack(lua, ss);
		am_log("LUAERR", ss);
	}

}
}
