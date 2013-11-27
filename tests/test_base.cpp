#include "test_base.h"

#include <tests/asserts.h>

namespace am {
namespace tests {

	int TestBase::TestManaged::getCounter() const {
		return getReferenceCounter();
	}

	bool TestBase::testSimple() {
		am::base::Handle<TestManaged> handled(new TestManaged());
		am_equals(1, handled->getCounter());

		handled->retain();
		am_equals(2, handled->getCounter());

		handled->release();
		am_equals(1, handled->getCounter());

		am::base::Handle<TestManaged> other(handled);
		am_equals(2, handled->getCounter());
		am_equals(2, other->getCounter());

		return true;
	}

}
}
