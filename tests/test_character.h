#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestCharacter : public TestSuite
	{
	public:
		TestCharacter() {}
		~TestCharacter() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
			runCase(testEquipped);
			runCase(testCopy);
			runCase(testDead);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::game::TestCharacter";
		}

	protected:

		bool testSimple();
		bool testEquipped();
		bool testCopy();
		bool testDead();
	
	};

}
}
