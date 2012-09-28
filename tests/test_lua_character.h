#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

extern "C"
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace tests {

	class TestLuaCharacter : public TestSuite {
	public:
		TestLuaCharacter() {}
		~TestLuaCharacter() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testGender);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::TestLuaCharacter";
		}

	protected:

		bool testSimple();
		bool testGender();

	};

}
}
