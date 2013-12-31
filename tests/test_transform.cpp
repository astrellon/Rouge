#include "test_transform.h"

#include <math/transform.h>
#include <math/matrix.h>
#include <math/vector.h>

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestTransform::testSimple()
	{
        float test[16] = {1.0f, 0, 0, 0,
            0, 1.0f, 0, 0,
            0, 0, 1.0f, 0,
            0, 0, 0, 1.0f};
        math::Transform trans;
        am_equalsArray(test, trans.data(), 16);

		trans.setPosition(0, 5, 0);
		trans.setTarget(0, 5, 5);
		float *data = trans.data();

        return true;
    }

}
}

