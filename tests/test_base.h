#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestBase : public TestSuite {
	public:
		TestBase() {}
		~TestBase() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::base";
		}

	protected:

		class TestManaged : public IManaged {
		public:
			TestManaged() {}
			~TestManaged() {}


			int getCounter() const;
		};

		bool testSimple();
	
	};

}
}
