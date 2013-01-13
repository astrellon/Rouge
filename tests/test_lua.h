#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <string>
using std::string;

namespace am {
namespace tests {

	namespace test {
		class Base {
		public:
			Base();
			~Base();

			void setName(const char *name);
			const char *getName() const;

			string name;
		};

		const char Base_tableName[] = "am_tests_test_Base";
		int Base_ctor(lua_State *lua);
		int Base_dtor(lua_State *lua);
		void Base_wrap(lua_State *lua, Base *base);

		int Base_set_name(lua_State *lua);
		int Base_get_name(lua_State *lua);

		int Base_register(lua_State *lua);
		Base *Check_Base(lua_State *lua, int n);

		class Child : public Base {
		public:
			Child();
			~Child();

			void setAge(int age);
			int getAge() const;

			int age;
		};

		const char Child_tableName[] = "am_tests_test_Child";
		int Child_ctor(lua_State *lua);
		int Child_dtor(lua_State *lua);
		void Child_wrap(lua_State *lua, Child *child);

		int Child_set_age(lua_State *lua);
		int Child_get_age(lua_State *lua);

		int Child_register(lua_State *lua);
		Child *Check_Child(lua_State *lua, int n);
	}

	class TestLua : public TestSuite {
	public:
		TestLua() {}
		~TestLua() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testScripts);
			runCase(testLoadScripts);
			runCase(testWrapper);
			//runCase(testInheritance);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::LuaState";
		}

	protected:

		bool testSimple();
		bool testScripts();
		bool testLoadScripts();
		bool testWrapper();
		bool testInheritance();
	
	};

}
}
