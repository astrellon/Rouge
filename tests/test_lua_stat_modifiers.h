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

	class TestLuaStatModifiers : public TestSuite {
	public:
		TestLuaStatModifiers() {}
		~TestLuaStatModifiers() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testSimple2);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::LuaStatModifiers";
		}

	protected:

		bool testSimple();
		bool testSimple2();

	};

}
}
