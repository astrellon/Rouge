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

	class TestLuaStats : public TestSuite 
	{
	public:
		TestLuaStats() {}
		~TestLuaStats() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::lua::game::TestLuaStats";
		}

	protected:

		bool testSimple();

	};

}
}
