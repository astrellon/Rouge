#include "math.h"

#include <cmath>

namespace am {
namespace math {

	double abs(double input)
	{
		return ::abs(input);
	}
	float abs(float input)
	{
		return ::abs(input);
	}
	int abs(int input)
	{
		return ::abs(input);
	}

	int round(double input)
	{
		return input < 0.0 ? ::ceil(input - 0.5) : ::floor(input + 0.5);
	}
	int round(float input)
	{
		return input < 0.0f ? ::ceilf(input - 0.5f) : ::floorf(input + 0.5f);
	}

}
}
