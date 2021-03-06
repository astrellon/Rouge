#include "data_boolean.h"

namespace am {
namespace util {
namespace data {

	const int Boolean::TYPE = 2;
	const char *Boolean::TYPENAME = "Boolean";

	Boolean::Boolean() :
		mValue(false)
	{
	}
	Boolean::Boolean(bool v) :
		mValue(v)
	{
	}
	Boolean::Boolean(const Boolean &copy) :
		mValue(copy.mValue)
	{
	}
	Boolean::~Boolean()
	{
	}

	IData *Boolean::clone() const
	{
		return new Boolean(*this);
	}

	void Boolean::value(bool v)
	{
		mValue = v;
	}
	bool Boolean::value() const
	{
		return mValue;
	}

	std::string Boolean::toLua() const
	{
		return mValue ? "true" : "false";
	}

	Boolean *Boolean::checkDataType(IData *data, const char *className)
	{
		return IData::checkDataType<Boolean>(data, className);
	}

}
}
}
