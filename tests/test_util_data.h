#pragma once

#include <base/handle.h>

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestUtilData: public TestSuite
	{
	public:
		TestUtilData() {}
		~TestUtilData() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
			runCase(testFromLua);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::util::data::IData";
		}

	protected:

		bool testSimple();
		bool testFromLua();
	
	};

}
}
