#pragma once

#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestSelectorValue : public TestSuite {
	public:
		TestSelectorValue() {}
		~TestSelectorValue() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::util::SelectorValue";
		}

	protected:

		bool testSimple();
	
	};

}
}
