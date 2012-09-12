#pragma once

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

#include <string>
#include <ostream>
using namespace std;

#include <base/imanaged.h>
using namespace am::base;

namespace am {
namespace lua {

	class LuaState : public IManaged {
	public:
		LuaState();
		~LuaState();

		lua_State *getLua();

		operator lua_State *();

		int newTable(const char *tableName = NULL);
		bool getTable(int tableRef);
		bool getTable(const char *tableName);
		bool compareRefs(int ref1, int ref2);

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

		static int onError(lua_State *mLua);

		void logStack(const char *cat);
		void printStack(ostream &output);

	protected:
		lua_State *mLua;

	};

}
}
