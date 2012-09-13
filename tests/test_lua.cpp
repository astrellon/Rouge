#include "test_lua.h"

#include <tests/asserts.h>

#include <lua/lua_state.h>
using namespace am::lua;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLua::testSimple() {
		LuaState lua;
		assert(lua.getLua() != NULL);

		int ref = lua.newTable("table1");
		lua.setTableValue("name", "Melli");
		lua.setTableValue("age", 22);

		lua.pop(1);

		assert(!lua_istable(lua, -1));
		lua.getTable("table1");
		assert(lua_istable(lua, -1));

		lua.pop(1);
		assert(!lua_istable(lua, -1));
		lua.getTable(ref);
		assert(lua_istable(lua, -1));

		const char *name = lua.getTableString("name");
		assert(name);
		assert(strcmp("Melli", name) == 0);
		int age = 0;
		assert(lua.getTableInt("age", age));
		equals(22, age);
		name = lua.getTableString("name");
		assert(name);
		assert(strcmp("Melli", name) == 0);

		lua.pop(1);

		// Should always have one function on the stack
		// which is the error handling function.
		equals(1, lua_gettop(lua));

		return true;
	}

	bool TestLua::testScripts()
	{
		LuaState lua;
		lua.loadString("function testFunc(x, y)\n"
			"	return x * y\n"
			"end");

		assert(lua.hasGlobalFunction("testFunc"));
		lua.push(4);
		lua.push(5);
		lua_call(lua, 2, 1);
		int result = lua_tointeger(lua, -1);
		equals(20, result);
		lua.pop(1);

		equals(1, lua_gettop(lua));
		assert(!lua.hasGlobalFunction("notafunc"));
		equals(1, lua_gettop(lua));

		return true;
	}

}
}