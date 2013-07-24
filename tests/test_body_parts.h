#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestBodyParts : public TestSuite {
	public:
		TestBodyParts() {}
		~TestBodyParts() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::game::BodyParts";
		}

	protected:

		bool testSimple();
	
	};

}
}
