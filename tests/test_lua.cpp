#include "test_lua.h"

#include <tests/asserts.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/character.h>
using namespace am::game;

#include <lua/wrappers/lua_character.h>
using namespace am::lua::game;

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
		assert(name != NULL);
		assert(strcmp("Melli", name) == 0);
		int age = 0;
		assert(lua.getTableInt("age", age));
		equals(22, age);
		name = lua.getTableString("name");
		assert(name != NULL);
		assert(strcmp("Melli", name) == 0);

		lua.pop(1);

		equals(0, lua_gettop(lua));

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

		equals(0, lua_gettop(lua));
		assert(!lua.hasGlobalFunction("notafunc"));
		equals(0, lua_gettop(lua));

		assert(lua.loadString("function notafunc()\n"
			"	return \"hello there\"\n"
			"end"));

		assert(lua.hasGlobalFunction("testFunc"));
		lua.push(4);
		lua.push(5);
		lua.call(2, 1);
		result = lua_tointeger(lua, -1);
		equals(20, result);
		lua.pop(1);

		equals(0, lua_gettop(lua));
		assert(lua.hasGlobalFunction("notafunc"));
		equals(1, lua_gettop(lua));

		lua.call(0, 1);
		const char *callResult = lua_tostring(lua, -1);
		assert(strcmp(callResult, "hello there") == 0);
		lua.pop(1);
		equals(0, lua_gettop(lua));

		// You can redefine functions!
		// And load them at run-time!
		// Must find a way of making use of this.
		assert(lua.loadString("function notafunc()\n"
			"	return \"how are you?\"\n"
			"end"));

		equals(0, lua_gettop(lua));
		assert(lua.hasGlobalFunction("notafunc"));
		equals(1, lua_gettop(lua));

		lua_call(lua, 0, 1);
		callResult = lua_tostring(lua, -1);
		assert(strcmp(callResult, "how are you?") == 0);
		lua.pop(1);
		equals(0, lua_gettop(lua));

		return true;
	}

	bool TestLua::testWrapper()
	{
		LuaState lua;
		Handle<Character> testCharacter(new Character());
		testCharacter->setGameId("testId");
		testCharacter->setName("Test Name");

		lua.loadString("Character = import(\"Character\")\n"
			"name = \"none\"\n"
			"function testFunc()\n"
			"	obj = Character.new(\"testId\")\n"
			"	name = obj:get_name()\n"
			"	obj:set_name(name..\" changed\")\n"
			"end");

		string name = lua.getGlobalString("name");
		assert(name.compare("none") == 0);

		assert(lua.hasGlobalFunction("testFunc"));
		lua.call(0, 0);

		name = lua.getGlobalString("name");
		assert(name.compare("Test Name") == 0);

		assert(testCharacter->getName().compare("Test Name changed") == 0);

		return true;
	}

}
}