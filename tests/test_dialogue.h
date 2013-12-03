#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestDialogue : public TestSuite 
	{
	public:
		TestDialogue() {}
		~TestDialogue() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const 
		{
			return "am::game::Dialogue";
		}

	protected:

		bool testSimple();
	
	};

}
}
