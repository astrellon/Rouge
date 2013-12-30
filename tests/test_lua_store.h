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

	class TestLuaStore : public TestSuite
	{
	public:
		TestLuaStore() {}
		~TestLuaStore() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::lua::game::Store";
		}

	protected:

		bool testSimple();

	};

}
}
