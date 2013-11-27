#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <tests/test_suite.h>

extern "C"
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace tests {

	class TestLuaSound : public TestSuite {
	public:
		TestLuaSound() {}
		~TestLuaSound() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::sfx::TestLuaSound";
		}

	protected:

		bool testSimple();

	};

}
}
