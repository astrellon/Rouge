#pragma once

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
#	include <lua/src/lstate.h>
} 

#include <string>
#include <map>
#include <ostream>
using namespace std;

#include <base/imanaged.h>
using namespace am::base;

#define lua_tofloat(L,i)	static_cast<float>(lua_tonumber(L,i))
#define lua_tobool(L,i)		(lua_toboolean(L,i) > 0)

namespace am {
namespace lua {

	template <class T>
	inline void newUData(lua_State *lua, T *obj)
	{
		int *udata = reinterpret_cast<int *>(lua_newuserdata(lua, sizeof(int) + sizeof(T *)));
		udata[0] = T::LUA_ID;
		udata[1] = reinterpret_cast<int>(obj);
	}

	template <class T>
	inline T *castUData(lua_State *lua, int n)
	{
		int *data = reinterpret_cast<int *>(lua_touserdata(lua, n));
		int id = data[0];
		if (id == T::LUA_ID)
		{
			return reinterpret_cast<T *>(data[1]);
		}
		return NULL;
	}
	int getUDataType(lua_State *lua, int n);

	template <class T>
	inline void wrapObject(lua_State *lua, T *object)
	{
		newUData<T>(lua, object);

		luaL_getmetatable(lua, T::LUA_TABLENAME);
		lua_setmetatable(lua, -2);
	}
	template <class T>
	inline void wrapRefObject(lua_State *lua, T *object)
	{
		newUData<T>(lua, object);

		object->retain();

		luaL_getmetatable(lua, T::LUA_TABLENAME);
		lua_setmetatable(lua, -2);
	}

	class LuaState : public IManaged {
	public:

		typedef map<string, lua_CFunction> WrapperMap;

		LuaState(bool includeLibraries = true);
		LuaState(lua_State *lua);
		~LuaState();

		lua_State *getLua();
		void close();

		bool loadFile(const char *filename);
		bool loadString(const char *luaString);

		void call(int n, int r);

		operator lua_State *();

		int newTable(const char *tableName = NULL);
		bool getTable(int tableRef);
		bool getTable(const char *tableName);
		bool compareRefs(int ref1, int ref2) const;

		void pop(int num);
		void push(const char *str);
		void push(int num);
		void push(double num);
		void push(bool val);
		void pushnil();

		void setTableValue(const char *key, const char *value);
		void setTableValue(const char *key, int value);
		void setTableValue(const char *key, double value);
		void setTableValue(const char *key, bool value);

		const char *getTableString(const char *key);
		bool getTableInt(const char *key, int &value);
		bool getTableDouble(const char *key, double &value);
		bool getTableBool(const char *key, bool &value);

		bool isTableNumber(const char *key);
		bool isTableString(const char *key);
		bool isTableTable(const char *key);
		bool isTableBool(const char *key);

		int hasTableValue(const char *key);

		float toNumber();
		int toInteger();
		bool toBool();

		bool hasGlobalFunction(const char *func, bool popAfter = false);
		int getGlobalInt(const char *name);
		double getGlobalDouble(const char *name);
		bool getGlobalBool(const char *name);
		string getGlobalString(const char *name);

		static int onError(lua_State *mLua);

		void logStack(const char *cat);
		void printStack(ostream &output);

		void logTable(const char *cat, int n);
		void printTable(ostream &output, int n);

		bool operator==(const lua_State *lua) const;
		bool operator==(const LuaState &rhs) const;
		bool operator!=(const lua_State *lua) const;
		bool operator!=(const LuaState &rhs) const;

		static void registerWrapper(const char *name, lua_CFunction call);
		static int getWrapper(lua_State *lua);
		static void clearRegistered();

		static void logStack(lua_State *lua, const char *cat);
		static void printStack(lua_State *lua, ostream &output);

		static int lua_am_log(lua_State *lua);

	protected:
		lua_State *mLua;

		static WrapperMap sWrapperMap;

		static int sDepth;
	};

}
}
