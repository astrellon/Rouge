#include "test_base.h"

#include <tests/asserts.h>

namespace am {
namespace tests {

	int TestBase::TestManaged::getCounter() const {
		return getReferenceCounter();
	}

	bool TestBase::testSimple() {
		Handle<TestManaged> handled(new TestManaged());
		equals(1, handled->getCounter());

		handled->retain();
		equals(2, handled->getCounter());

		handled->release();
		equals(1, handled->getCounter());

		Handle<TestManaged> other(handled);
		equals(2, handled->getCounter());
		equals(2, other->getCounter());

		return true;
	}

}
}
