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
			Base_wrap(lua, base);
			return 1;
		}
		int Base_dtor(lua_State *lua)
		{
			Base *base = Check_Base(lua, 1);
			if (base)
			{
				delete base;
			}
			return 0;
		}
		void Base_wrap(lua_State *lua, Base *base)
		{
			Base ** udata = (Base **)lua_newuserdata(lua, sizeof(Base *));
			*udata = base;

			luaL_getmetatable(lua, Base_tableName);
			lua_setmetatable(lua, -2);
		}

		int Base_set_name(lua_State *lua)
		{
			Base *base = Check_Base(lua, 1);
			if (base && lua_isstring(lua, 2))
			{
				base->setName(lua_tostring(lua, 2));
			}
			return 0;
		}
		int Base_get_name(lua_State *lua)
		{
			Base *base = Check_Base(lua, 1);
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
				{ NULL, NULL }
			};

			luaL_newmetatable(lua, Base_tableName);
			luaL_setfuncs(lua, regs, 0);

			lua_pushvalue(lua, -1);
			lua_setfield(lua, -1, "__index");
		
			return 1;
		}
		Base *Check_Base(lua_State *lua, int n)
		{
			return *(Base **)luaL_checkudata(lua, n, Base_tableName);
		}

		Child::Child() : Base(), age(0) {}
		Child::~Child() {}

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
			Child_wrap(lua, child);
			return 1;
		}
		int Child_dtor(lua_State *lua)
		{
			Child *child = Check_Child(lua, 1);
			if (child)
			{
				delete child;
			}
			return 0;
		}

		void Child_wrap(lua_State *lua, Child *child)
		{
			Child ** udata = (Child **)lua_newuserdata(lua, sizeof(Child *));
			*udata = child;

			luaL_getmetatable(lua, Child_tableName);
			lua_setmetatable(lua, -2);
		}

		int Child_set_age(lua_State *lua)
		{
			Child *child = Check_Child(lua, 1);
			if (child && lua_isnumber(lua, 2))
			{
				child->setAge(lua_tointeger(lua, 2));
			}
			return 0;
		}
		int Child_get_age(lua_State *lua)
		{
			Child *child = Check_Child(lua, 1);
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
				{ NULL, NULL }
			};

			luaL_newmetatable(lua, Child_tableName);
			luaL_setfuncs(lua, regs, 0);

			lua_pushstring(lua, "__index");
			luaL_getmetatable(lua, Base_tableName);
			lua_settable(lua, -3);
			
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -1, "__index");
		
			return 1;
		}
		Child *Check_Child(lua_State *lua, int n)
		{
			return *(Child **)luaL_checkudata(lua, n, Child_tableName);
		}
	}

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
		equalsStr("Melli", name);
		int age = 0;
		assert(lua.getTableInt("age", age));
		equals(22, age);
		name = lua.getTableString("name");
		assert(name != NULL);
		equalsStr("Melli", name);

		lua.pop(1);

		equals(0, lua_gettop(lua));

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
		equalsStr(callResult, "hello there");
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
		equalsStr(callResult, "how are you?");
		lua.pop(1);
		equals(0, lua_gettop(lua));

		lua.close();

		return true;
	}

	bool TestLua::testWrapper()
	{
		LuaState lua;

		Engine::getEngine()->setCurrentGame(new Game());
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
		equalsStr("none", name.c_str());

		assert(lua.hasGlobalFunction("testFunc"));
		lua.call(0, 0);

		name = lua.getGlobalString("name");
		equalsStr("Test Name", name.c_str());

		equalsStr("Test Name changed", testCharacter->getName().c_str());

		lua.close();
		return true;
	}

	bool TestLua::testInheritance()
	{
		LuaState lua;

		lua.registerWrapper("Base", test::Base_register);
		lua.registerWrapper("Child", test::Child_register);

		int loadResult = lua.loadString("Base, Child = import(\"Base\", \"Child\")\n"
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
		lua.call(0, 1);
		test::Base *base = test::Check_Base(lua, -1);
		assert(base != NULL);

		equalsStr("Melli", base->getName());

		assert(lua.hasGlobalFunction("getChild"));
		lua.call(0, 1);
		test::Child *child = test::Check_Child(lua, -1);
		assert(child != NULL);

		equals(23, child->getAge());

		lua.close();

		return true;
	}

}
}