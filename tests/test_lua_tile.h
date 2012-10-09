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

	class TestLuaTile : public TestSuite {
	public:
		TestLuaTile() {}
		~TestLuaTile() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::TestLuaTile";
		}

	protected:

		bool testSimple();

	};

}
}
