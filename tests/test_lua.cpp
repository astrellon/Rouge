#include "test_lua.h"

#include <tests/asserts.h>

#include <game/character.h>
#include <game/game.h>
#include <game/engine.h>
using namespace am::game;

#include <lua/wrappers/game/lua_character.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	namespace test {
		Base::Base() : name("Unknown") {}
		Base::~Base() {}

		const char *Base::LUA_TABLENAME = "am_tests_test_Base";
		int Base::LUA_ID = 5000;

		void Base::setName(const char *name)
		{
			this->name = name;
		}
		const char *Base::getName() const
		{
			return name.c_str();
		}

		int Base_ctor(lua_State *lua)
		{
			Base *base = new Base();
			wrapObject<Base>(lua, base);
			return 1;
		}
		int Base_dtor(lua_State *lua)
		{
			Base *base = castUData<Base>(lua, 1);
			if (base)
			{
				delete base;
			}
			return 0;
		}

		int Base_set_name(lua_State *lua)
		{
			Base *base = castUData<Base>(lua, 1);
			if (base && lua_isstring(lua, 2))
			{
				base->setName(lua_tostring(lua, 2));
			}
			return 0;
		}
		int Base_get_name(lua_State *lua)
		{
			Base *base = castUData<Base>(lua, 1);
			if (base)
			{
				lua_pushstring(lua, base->getName());
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}

		int Base_register(lua_State *lua)
		{
			luaL_Reg regs[] = 
			{
				{ "new", Base_ctor },
				{ "__gc", Base_dtor },
				{ "set_name", Base_set_name },
				{ "get_name", Base_get_name },
				{ nullptr, nullptr }
			};

			luaL_newmetatable(lua, Base::LUA_TABLENAME);
			luaL_setfuncs(lua, regs, 0);

			lua_pushvalue(lua, -1);
			lua_setfield(lua, -1, "__index");
		
			return 1;
		}

		Child::Child() : Base(), age(0) {}
		Child::~Child() {}

		const char *Child::LUA_TABLENAME = "am_tests_test_Child";
		int Child::LUA_ID = 5001;

		void Child::setAge(int age)
		{
			this->age = age;
		}
		int Child::getAge() const
		{
			return age;
		}

		int Child_ctor(lua_State *lua)
		{
			Child *child = new Child();
			wrapObject<Child>(lua, child);
			return 1;
		}
		int Child_dtor(lua_State *lua)
		{
			Child *child = castUData<Child>(lua, 1);
			if (child)
			{
				delete child;
			}
			return 0;
		}

		int Child_set_age(lua_State *lua)
		{
			Child *child = castUData<Child>(lua, 1);
			if (child && lua_isnumber(lua, 2))
			{
				child->setAge(lua_tointeger(lua, 2));
			}
			return 0;
		}
		int Child_get_age(lua_State *lua)
		{
			Child *child = castUData<Child>(lua, 1);
			if (child)
			{
				lua_pushinteger(lua, child->getAge());
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}

		int Child_register(lua_State *lua)
		{
			luaL_Reg regs[] = 
			{
				{ "new", Child_ctor },
				{ "__gc", Child_dtor },
				{ "set_age", Child_set_age },
				{ "get_age", Child_get_age},
				{ nullptr, nullptr }
			};

			luaL_newmetatable(lua, Child::LUA_TABLENAME);
			luaL_setfuncs(lua, regs, 0);

			lua_pushstring(lua, "__index");
			luaL_getmetatable(lua, Base::LUA_TABLENAME);
			lua_settable(lua, -3);
			
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -1, "__index");
		
			return 1;
		}
	}

	bool TestLua::testSimple() {
		LuaState lua;
		assert(lua.getLua() != nullptr);

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
		assert(name != nullptr);
		am_equalsStr("Melli", name);
		int age = 0;
		assert(lua.getTableInt("age", age));
		am_equals(22, age);
		name = lua.getTableString("name");
		assert(name != nullptr);
		am_equalsStr("Melli", name);

		lua.pop(1);

		am_equals(0, lua_gettop(lua));

		lua.close();

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
		lua_acall(lua, 2, 1);
		int result = lua_tointeger(lua, -1);
		am_equals(20, result);
		lua.pop(1);

		am_equals(0, lua_gettop(lua));
		assert(!lua.hasGlobalFunction("notafunc"));
		am_equals(0, lua_gettop(lua));

		assert(lua.loadString("function notafunc()\n"
			"	return \"hello there\"\n"
			"end"));

		assert(lua.hasGlobalFunction("testFunc"));
		lua.push(4);
		lua.push(5);
		lua_acall(lua, 2, 1);
		result = lua_tointeger(lua, -1);
		am_equals(20, result);
		lua.pop(1);

		am_equals(0, lua_gettop(lua));
		assert(lua.hasGlobalFunction("notafunc"));
		am_equals(1, lua_gettop(lua));

		lua_acall(lua, 0, 1);
		const char *callResult = lua_tostring(lua, -1);
		am_equalsStr(callResult, "hello there");
		lua.pop(1);
		am_equals(0, lua_gettop(lua));

		// You can redefine functions!
		// And load them at run-time!
		// Must find a way of making use of this.
		assert(lua.loadString("function notafunc()\n"
			"	return \"how are you?\"\n"
			"end"));

		am_equals(0, lua_gettop(lua));
		assert(lua.hasGlobalFunction("notafunc"));
		am_equals(1, lua_gettop(lua));

		lua_acall(lua, 0, 1);
		callResult = lua_tostring(lua, -1);
		am_equalsStr(callResult, "how are you?");
		lua.pop(1);
		am_equals(0, lua_gettop(lua));

		lua.close();

		return true;
	}

	bool TestLua::testLoadScripts()
	{
		LuaState lua;
		bool loaded = lua.loadString("name = \"Melli\"\n"
			"local age = 23\n"
			"function getName()\n"
			"	return name\n"
			"end\n"
			"local function getAge()\n"
			"	return age\n"
			"end\n"
			"function getNameAndAge()\n"
			"	return getName(), getAge()\n"
			"end");
		if (!loaded) 
		{
			lua.logStack("ERR");
			return false;
		}

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("Melli", lua_tostring(lua, -1));
		lua.pop(1);

		assert(!lua.hasGlobalFunction("getAge"));
		assert(lua.hasGlobalFunction("getNameAndAge"));
		lua_acall(lua, 0, 2);
		am_equalsStr("Melli", lua_tostring(lua, -2));
		am_equals(23, lua_tointeger(lua, -1));

		loaded = lua.loadString("name = \"Alan\"\n"
			"local age = 24\n"
			"local function getAge()\n"
			"	return age\n"
			"end\n");
		if (!loaded)
		{
			lua.logStack("ERR");
			return false;
		}

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("Alan", lua_tostring(lua, -1));
		lua.pop(1);

		assert(!lua.hasGlobalFunction("getAge"));
		assert(lua.hasGlobalFunction("getNameAndAge"));
		lua_acall(lua, 0, 2);
		am_equalsStr("Alan", lua_tostring(lua, -2));
		am_equals(23, lua_tointeger(lua, -1));

		return true;
	}

	bool TestLua::testWrapper()
	{
		LuaState lua;

		Engine::getEngine()->setCurrentGame(new Game());
		Handle<Character> testCharacter(new Character());
		testCharacter->setGameId("testId");
		testCharacter->setName("Test Name");

		/*lua.loadString("Character = import(\"Character\")\n"
			"name = \"none\"\n"
			"function testFunc()\n"
			"	obj = Character.new(\"testId\")\n"
			"	name = obj:name()\n"
			"	obj:name(name..\" changed\")\n"
			"end");*/
		bool loadResult = lua.loadString(
			"name = \"none\"\n"
			"function testFunc()\n"
			"	am.chara = {}\n"
			"	obj = am.character.new(\"testId\")\n"
			"	name = obj:name()\n"
			"	obj:name(name..\" changed\")\n"
			"end");

		if (!loadResult)
		{
			lua.logStack("LUAERR");
			assert(loadResult);
		}

		string name = lua.getGlobalString("name");
		am_equalsStr("none", name.c_str());

		assert(lua.hasGlobalFunction("testFunc"));
		try 
		{
			lua_acall(lua, 0, 0);
		}
		catch (...)
		{
			lua.logStack("LUAERR");
			return false;
		}

		name = lua.getGlobalString("name");
		am_equalsStr("Test Name", name.c_str());

		am_equalsStr("Test Name changed", testCharacter->getName().c_str());

		lua.close();
		return true;
	}

	bool TestLua::testInheritance()
	{
		/*LuaState lua;

		lua.registerWrapper("Base", test::Base::LUA_ID);
		lua.registerWrapper("Child", test::Child::LUA_ID);

		int loadResult = lua.loadString(
			"base = Base.new()\n"
			"base:set_name(\"Melli\")\n"
			"child = Child.new()\n"
			"child:set_age(23)\n"
			"child:set_name(\"Alan\")\n"
			"function getBase()\n"
			"	return base\n"
			"end\n"
			"function getChild()\n"
			"	return child\n"
			"end\n"
			);

		if (!loadResult)
		{
			lua.logStack("ERR LOADING");
			return false;
		}

		assert(lua.hasGlobalFunction("getBase"));
		lua_acall(lua, 0, 1);
		test::Base *base = castUData<test::Base>(lua, -1);
		assert(base != nullptr);

		am_equalsStr("Melli", base->getName());

		assert(lua.hasGlobalFunction("getChild"));
		lua_acall(lua, 0, 1);
		test::Child *child = castUData<test::Child>(lua, -1);
		assert(child != nullptr);

		am_equals(23, child->getAge());

		lua.close();*/

		return true;
	}

	bool TestLua::testReturnCodes()
	{
		LuaState lua;
		if (!lua.loadString(
			"am.debug.equals(1, am.code.success)\n"
			"am.debug.equals(\"success\", am.code.message(am.code.success))\n"
			))
		{
			lua.logStack("LUAERR");
			return false;
		}

		return true;
	}

}
}