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

	class TestLuaItem : public TestSuite {
	public:
		TestLuaItem() {}
		~TestLuaItem() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testBodyPartTypes);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::Item";
		}

	protected:

		bool testSimple();
		bool testBodyPartTypes();

	};

}
}
