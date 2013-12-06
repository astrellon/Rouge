#pragma once

namespace am {
namespace math {

    extern const double PI; 

	double abs(double input);
	float abs(float input);
	int abs(int input);

	int round(double input);
	int round(float input);

	template <class T>
	const T &maxv(const T &a, const T &b)
	{
		return (a < b) ? b : a;
	}
	template <class T>
	const T &minv(const T &a, const T &b)
	{
		return (a < b) ? a : b;
	}
}
}
