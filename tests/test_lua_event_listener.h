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

	class TestLuaEventListener : public TestSuite {
	public:
		TestLuaEventListener() {}
		~TestLuaEventListener() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testMouse);
		}
	
		virtual const char *getSuiteName() const {
			return "am::ui::LuaEventListener";
		}

	protected:

		bool testSimple();
		bool testMouse();

	};

}
}
