#include "data_integer.h"

#include <sstream>

namespace am {
namespace util {
namespace data {

	const int Integer::TYPE = 5;
	const char *Integer::TYPENAME = "Integer";

	Integer::Integer() :
		mValue(0)
	{
	}
	Integer::Integer(int v) :
		mValue(v)
	{
	}
	Integer::~Integer()
	{
	}

	void Integer::value(int v)
	{
		mValue = v;
	}
	int Integer::value() const
	{
		return mValue;
	}

	std::string Integer::toLua() const
	{
		std::stringstream ss;
		ss << mValue;
		return ss.str();
	}

}
}
}
