#pragma once

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestStore : public TestSuite
	{
	public:
		TestStore() {}
		~TestStore() {}
	
		virtual void runCases()
		{
			runCase(testBuy);
			runCase(testSell);
			runCase(testSell2);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::game::Store";
		}

	protected:

		bool testBuy();
		bool testSell();
		bool testSell2();
	
	};

}
}
