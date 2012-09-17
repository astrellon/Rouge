#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

extern "C"
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace tests {

	class TestLuaQuest : public TestSuite {
	public:
		TestLuaQuest() {}
		~TestLuaQuest() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::LuaQuest";
		}

		static int getLevel(lua_State *lua);

	protected:

		bool testSimple();

		static int sLevel;
	};

}
}
