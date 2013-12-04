#include "math.h"

#include <cmath>

namespace am {
namespace math {

    const double PI = 3.14159265359; 

	double abs(double input)
	{
		return std::abs(input);
	}
	float abs(float input)
	{
		return std::abs(input);
	}
	int abs(int input)
	{
		return std::abs(input);
	}

	int round(double input)
	{
		return input < 0.0 ? std::ceil(input - 0.5) : std::floor(input + 0.5);
	}
	int round(float input)
	{
		return input < 0.0f ? std::ceil(input - 0.5f) : std::floor(input + 0.5f);
	}

}
}
