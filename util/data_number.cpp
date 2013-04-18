#include "data_number.h"

#include <sstream>

#include <math/math.h>

namespace am {
namespace util {
namespace data {

	const int Number::TYPE = 1;
	const char *Number::TYPENAME = "Number";

	Number::Number() :
		mValue(0.0)
	{
	}
	Number::Number(int v) :
		mValue(v)
	{
	}
	Number::Number(double v) :
		mValue(v)
	{
	}
	Number::~Number()
	{
	}

	void Number::value(double v)
	{
		mValue = v;
	}
	void Number::value(int v)
	{
		mValue = static_cast<double>(v);
	}
	double Number::value() const
	{
		return mValue;
	}
	int Number::valuei() const
	{
		return am::math::round(mValue);
	}

	std::string Number::toLua() const
	{
		std::stringstream ss;
		ss << mValue;
		return ss.str();
	}

	Number *Number::checkDataType(IData *data, const char *className)
	{
		return IData::checkDataType<Number>(data, className);
	}

}
}
}
