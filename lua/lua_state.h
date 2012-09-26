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

namespace am {
namespace lua {

	class LuaState : public IManaged {
	public:

		typedef map<string, lua_CFunction> WrapperMap;

		LuaState();
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

		bool hasGlobalFunction(const char *func, bool popAfter = false);
		int getGlobalInt(const char *name);
		double getGlobalDouble(const char *name);
		bool getGlobalBool(const char *name);
		string getGlobalString(const char *name);

		static int onError(lua_State *mLua);

		void logStack(const char *cat);
		void printStack(ostream &output);

		bool operator==(const lua_State *lua) const;
		bool operator==(const LuaState &rhs) const;
		bool operator!=(const lua_State *lua) const;
		bool operator!=(const LuaState &rhs) const;

		static void registerWrapper(const char *name, lua_CFunction call);
		static int getWrapper(lua_State *lua);

		static void logStack(lua_State *lua, const char *cat);
		static void printStack(lua_State *lua, ostream &output);

		static int lua_am_log(lua_State *lua);

	protected:
		lua_State *mLua;

		static WrapperMap sWrapperMap;
	};

}
}
