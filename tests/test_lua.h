#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestLua : public TestSuite {
	public:
		TestLua() {}
		~TestLua() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testScripts);
			runCase(testWrapper);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::LuaState";
		}

	protected:

		bool testSimple();
		bool testScripts();
		bool testWrapper();
	
	};

}
}
