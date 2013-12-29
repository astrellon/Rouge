#include "test_transform.h"

#include <math/transform.h>
#include <math/matrix.h>
#include <math/vector.h>

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestTransform::testSimple()
	{
        math::Vector2f vec(1.0f, 2.0f);
        am_equalsVec(vec, math::Vector2f(1.0f, 2.0f));
        return true;
    }

}
}

