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

	class TestLuaAsset: public TestSuite {
	public:
		TestLuaAsset() {}
		~TestLuaAsset() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::gfx::Asset";
		}

	protected:

		bool testSimple();

	};

}
}
