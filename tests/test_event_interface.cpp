#include "test_event_interface.h"

#include <ui/event_interface.h>
#include <ui/event_manager.h>
using namespace am::ui;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestEventInterface::testSimple() {
		retain();

		base::Handle<EventInterface> inter(new EventInterface());
		mEventCounter = 0;

		assert(!inter->hasActiveManager());
		assert(!inter->hasEventListener("test"));

		try 
		{
			base::Handle<Event> e(new Event("test"));
			inter->fireEvent<Event>(e);
			am_equals(0, mEventCounter);
		}
		catch (...) 
		{
			assert(false);
		}

		inter->addEventListener("test", this);
		assert(inter->hasActiveManager());

		try 
		{
			base::Handle<Event> e(new Event("test"));
			inter->fireEvent<Event>(e);
			am_equals(1, mEventCounter);
		}
		catch (...) 
		{
			assert(false);
		}

		inter->removeEventListener("test", this);
		assert(!inter->hasActiveManager());

		try 
		{
			base::Handle<Event> e(new Event("test"));
			inter->fireEvent<Event>(e);
			am_equals(1, mEventCounter);
		}
		catch (...) 
		{
			assert(false);
		}

		return true;
	}

	void TestEventInterface::onEvent(ui::Event *e)
	{
		mEventCounter++;
	}


}
}
