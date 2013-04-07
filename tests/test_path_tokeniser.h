#pragma once

#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestPathTokeniser : public TestSuite {
	public:
		TestPathTokeniser() {}
		~TestPathTokeniser() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::util::PathTokeniser";
		}

	protected:

		bool testSimple();
	
	};

}
}
