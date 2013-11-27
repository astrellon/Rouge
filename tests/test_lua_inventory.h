#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <ui/ievent_listener.h>
using namespace am::ui;

#include <tests/test_suite.h>

extern "C"
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace tests {

	class TestLuaInventory : public TestSuite {
	public:
		TestLuaInventory() {}
		~TestLuaInventory() {}
	
		virtual void runCases() {
			runCase(testSimple);
			runCase(testSpots);
			runCase(testEvents);
		}
	
		virtual const char *getSuiteName() const {
			return "am::lua::game::Inventory";
		}

	protected:

		bool testSimple();
		bool testSpots();
		bool testEvents();

		class TestHandler : public IEventListener {
		public:
			TestHandler();

			virtual void onEvent(InventoryEvent *e);

			bool accept;
			int counter;
		};

	};

}
}
