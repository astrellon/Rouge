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

	class TestLuaInventory : public TestSuite {
	public:
		TestLuaInventory() {}
		~TestLuaInventory() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testSpots);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::Inventory";
		}

	protected:

		bool testSimple();
		bool testSpots();

	};

}
}
