#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestItem : public TestSuite 
	{
	public:
		TestItem() {}
		~TestItem() {}
	
		virtual void runCases() 
		{
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const 
		{
			return "am::game::Item";
		}

	protected:

		bool testSimple();
	
	};

}
}
