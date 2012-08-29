#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestSelector : public TestSuite {
	public:
		TestSelector() {}
		~TestSelector() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testComplex);
		}
	
		virtual const char *getSuiteName() const {
			return "am::base TestSelector";
		}

	protected:

		bool testSimple();
		bool testComplex();
	
	};

}
}
