#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestMap : public TestSuite {
	public:
		TestMap() {}
		~TestMap() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testValidTiles);
		}
	
		virtual const char *getSuiteName() const {
			return "am::game::Map";
		}

	protected:

		bool testSimple();
		bool testValidTiles();
	
	};

}
}
