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

	class TestLuaEngine : public TestSuite {
	public:
		TestLuaEngine() {}
		~TestLuaEngine() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testTiles);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::Engine";
		}

	protected:

		bool testSimple();
		bool testTiles();

	};

}
}
