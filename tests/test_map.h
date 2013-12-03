#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

#include <ui/ievent_listener.h>

#include <tests/test_suite.h>

namespace am {
namespace tests {

	class TestMap : public TestSuite
	{
	public:
		TestMap() {}
		~TestMap() {}
	
		virtual void runCases() 
		{
			runCase(testSimple);
			runCase(testValidTiles);
			runCase(testValidTilesBig);
			runCase(testEdgeValue);
			runCase(testMapRegions);
		}
	
		virtual const char *getSuiteName() const
		{
			return "am::game::Map";
		}

	protected:

		bool testSimple();
		bool testValidTiles();
		bool testValidTilesBig();
		bool testEdgeValue();
		bool testMapRegions();

		class TestMapListener : public ui::IEventListener 
		{
		public:

			TestMapListener();

			virtual void onEvent(ui::MapRegionEvent *e);
			
			int counter;
		
		};

	};

}
}
