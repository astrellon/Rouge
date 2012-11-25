#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <tests/test_suite.h>

#include <ui/ievent_listener.h>
#include <ui/event.h>
using am::ui::IEventListener;
using am::ui::Event;

namespace am {
namespace tests {

	class TestEventInterface : public TestSuite, public IEventListener {
	public:
		TestEventInterface() :
			mEventCounter(0) {}
		~TestEventInterface() {}
	
		virtual void runCases() {
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const {
			return "am::ui::EventInterface";
		}

		virtual void onEvent(Event *e);

	protected:

		int mEventCounter;
		bool testSimple();
	
	};

}
}
