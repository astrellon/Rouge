#pragma once

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestTransform : public TestSuite
	{
	public:
		TestTransform() {}
		~TestTransform() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::math::Tranform";
		}

	protected:

		bool testSimple();
	
	};

}
}
