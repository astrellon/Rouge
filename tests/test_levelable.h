#pragma once

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestLevelable : public TestSuite 
	{
	public:
		TestLevelable() {}
		~TestLevelable() {}
	
		virtual void runCases() 
		{
			runCase(testSimple);
			runCase(testMaxLevel);
		}
	
		virtual const char *getSuiteName() const 
		{
			return "am::game::Levelable";
		}

	protected:

		bool testSimple();
		bool testMaxLevel();
	
	};

}
}
