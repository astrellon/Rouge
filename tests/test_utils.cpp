#include "test_utils.h"

#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include <tests/asserts.h>

#include <util/utils.h>
using namespace am::util;

namespace am {
namespace tests {

	bool TestUtils::testVectorFind() {
		vector<int> testVector;
		for (int i = 0; i < 4; i++)
		{
			testVector.push_back(i);
		}
		am_equals(-1, Utils::find(testVector, 4));
		am_equals(2, Utils::find(testVector, 2));
		am_equals(0, Utils::find(testVector, 0));

		return true;
	}

}
}
