#pragma once

#include <base/handle.h>

#include <tests/test_suite.h>

#include <ui/ievent_listener.h>
#include <ui/mouse_event.h>
using namespace am::ui;

namespace am {
namespace tests {

	class TestMouseManager : public TestSuite {
	public:
		TestMouseManager() {}
		~TestMouseManager() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::ui::MouseManager";
		}

	protected:

		class TestListener : public IEventListener {
		public:

			virtual void onEvent(MouseEvent *e);

			am::base::Handle<MouseEvent> lastEvent;
		};

		bool testSimple();
	
	};

}
}
