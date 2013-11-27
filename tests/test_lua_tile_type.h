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

	class TestLuaTileType : public TestSuite {
	public:
		TestLuaTileType() {}
		~TestLuaTileType() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testStatics);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::TestLuaTileType";
		}

	protected:

		bool testSimple();
		bool testStatics();

	};

}
}
