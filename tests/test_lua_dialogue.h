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

	class TestLuaDialogue : public TestSuite {
	public:
		TestLuaDialogue() {}
		~TestLuaDialogue() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::TestLuaDialogue";
		}

	protected:

		bool testSimple();

	};

}
}
