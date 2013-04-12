#include "data_number.h"

#include <sstream>

namespace am {
namespace util {
namespace data {

	const int Number::TYPE = 1;
	const char *Number::TYPENAME = "Number";

	Number::Number() :
		mValue(0.0)
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
	double Number::value() const
	{
		return mValue;
	}

	std::string Number::toLua() const
	{
		std::stringstream ss;
		ss << mValue;
		return ss.str();
	}

}
}
}
