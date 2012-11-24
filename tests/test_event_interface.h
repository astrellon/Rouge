#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestEventInterface : public TestSuite {
	public:
		TestEventInterface() {}
		~TestEventInterface() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::game::EventInterface";
		}

	protected:

		bool testSimple();
	
	};

}
}
