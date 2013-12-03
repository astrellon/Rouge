#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <tests/test_suite.h>

#include <lua/lua_state.h>

#include <string>

namespace am {
namespace tests {

	namespace test {
		class Base 
		{
		public:
			Base();
			~Base();

			void setName(const char *name);
			const char *getName() const;

			std::string name;

			static const char *LUA_TABLENAME;
			static int LUA_ID;
		};

		int Base_ctor(lua_State *lua);
		int Base_dtor(lua_State *lua);

		int Base_set_name(lua_State *lua);
		int Base_get_name(lua_State *lua);

		int Base_register(lua_State *lua);

		class Child : public Base
		{
		public:
			Child();
			~Child();

			void setAge(int age);
			int getAge() const;

			int age;

			static const char *LUA_TABLENAME;
			static int LUA_ID;
		};

		int Child_ctor(lua_State *lua);
		int Child_dtor(lua_State *lua);

		int Child_set_age(lua_State *lua);
		int Child_get_age(lua_State *lua);

		int Child_register(lua_State *lua);
	}

	class TestLua : public TestSuite
	{
	public:
		TestLua() {}
		~TestLua() {}
	
		virtual void runCases() 
		{
			runCase(testSimple);
			runCase(testScripts);
			runCase(testLoadScripts);
			runCase(testWrapper);
			runCase(testReturnCodes);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::lua::LuaState";
		}

	protected:

		bool testSimple();
		bool testScripts();
		bool testLoadScripts();
		bool testWrapper();
		bool testInheritance();
		bool testReturnCodes();

	};

}
}
