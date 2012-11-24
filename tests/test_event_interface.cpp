#include "test_event_interface.h"

#include <ui/event_interface.h>
#include <ui/event_manager2.h>
using namespace am::ui;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestEventInterface::testSimple() {

		EventInterface inter;
		assert(!inter.hasEventListener("test"));
		
		return true;
	}


}
}
