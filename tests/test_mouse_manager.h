#pragma once

#include <base/handle.h>

#include <tests/test_suite.h>

#include <ui/ievent_listener.h>
#include <ui/mouse_event.h>

namespace am {
namespace tests {

	class TestMouseManager : public TestSuite
	{
	public:
		TestMouseManager() {}
		~TestMouseManager() {}
	
		virtual void runCases()
		{
			runCase(testSimple);
		}
	
		virtual const char *getSuiteName() const 
		{
			return "ui::MouseManager";
		}

	protected:

		class TestListener : public ui::IEventListener 
		{
		public:

			virtual void onEvent(ui::MouseEvent *e);

			base::Handle<ui::MouseEvent> lastEvent;
		};

		bool testSimple();
	
	};

}
}
