#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <tests/test_suite.h>

#include <ui/ievent_listener.h>
#include <ui/event_interface.h>
#include <ui/event.h>

namespace am {
namespace tests {

	class TestEventInterface : public TestSuite, public ui::EventInterface, public ui::IEventListener 
	{
	public:
		TestEventInterface() :
			IEventListener(),
			mEventCounter(0) {}
		~TestEventInterface() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const 
		{
			return "ui::EventInterface";
		}

		virtual void onEvent(ui::Event *e);

	protected:

		int mEventCounter;
		bool testSimple();
	
	};

}
}
