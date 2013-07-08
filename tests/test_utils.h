#pragma once

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestUtils: public TestSuite {
	public:
		TestUtils() {}
		~TestUtils() {}
	
		virtual void runCases() {
			runCase(testVectorFind);
		}
	
		virtual const char *getSuiteName() const {
			return "am::util::Utils";
		}

	protected:

		bool testVectorFind();
	
	};

}
}
