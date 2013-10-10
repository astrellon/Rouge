#pragma once

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestStore : public TestSuite {
	public:
		TestStore() {}
		~TestStore() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::game::Store";
		}

	protected:

		bool testSimple();
	
	};

}
}
