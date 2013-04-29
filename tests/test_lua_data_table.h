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

	class TestLuaDataTable : public TestSuite {
	public:
		TestLuaDataTable() {}
		~TestLuaDataTable() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::util::data::DataTable";
		}

	protected:

		bool testSimple();

	};

}
}
