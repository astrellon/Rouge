#include "data_string.h"

namespace am {
namespace util {
namespace data {

	const int String::TYPE = 3;
	const char *String::TYPENAME = "String";

	String::String()
	{
	}
	String::String(const char *v) :
		mValue(v)
	{
	}
	String::~String()
	{
	}

	void String::value(const char *v)
	{
		mValue = v;
	}
	const char *String::value() const
	{
		return mValue.c_str();
	}

	std::string String::toLua() const
	{
		std::string str("\"");
		str += mValue;
		str += "\"";
		return str;
	}

}
}
}
