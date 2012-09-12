#pragma once

#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestJsonValue : public TestSuite {
	public:
		TestJsonValue() {}
		~TestJsonValue() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testArrays);
			runCase(testCastFloat);
		}
	
		virtual const char *getSuiteName() const {
			return "am::util::JsonValue";
		}

	protected:

		bool testSimple();
		bool testArrays();
		bool testCastFloat();
	
	};

}
}
