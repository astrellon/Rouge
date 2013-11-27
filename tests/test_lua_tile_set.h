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

	class TestLuaTileSet : public TestSuite {
	public:
		TestLuaTileSet() {}
		~TestLuaTileSet() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testTiles);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::TestLuaTileSet";
		}

	protected:

		bool testSimple();
		bool testTiles();

	};

}
}
