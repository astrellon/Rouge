#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestCharacter : public TestSuite {
	public:
		TestCharacter() {}
		~TestCharacter() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testEquipped);
			runCase(testSelector);
		}
	
		virtual const char *getSuiteName() const {
			return "am::base TestCharacter";
		}

	protected:

		bool testSimple();
		bool testEquipped();
		bool testSelector();
	
	};

}
}
