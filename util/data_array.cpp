#include "data_array.h"

#include "data_boolean.h"
#include "data_number.h"
#include "data_string.h"

namespace am {
namespace util {
namespace data {

	const int Array::TYPE = 4;
	const char *Array::TYPENAME = "Array";

	Array::Array()
	{
	}
	Array::~Array()
	{
	}

	Array::iterator Array::begin()
	{
		return mValue.begin();
	}
	Array::iterator Array::end()
	{
		return mValue.end();
	}

	void Array::push(IData *v)
	{
		mValue.push_back(v);
	}
	void Array::push(double v)
	{
		mValue.push_back(new Number(v));
	}
	void Array::push(int v)
	{
		mValue.push_back(new Number(v));
	}
	void Array::push(bool v)
	{
		mValue.push_back(new Boolean(v));
	}
	void Array::push(const char *v)
	{
		mValue.push_back(new String(v));
	}
	void Array::push(const std::string &v)
	{
		mValue.push_back(new String(v.c_str()));
	}

	IData *Array::operator[](unsigned int index)
	{
		return mValue[index];
	}

	Array::Array_internal &Array::inner()
	{
		return mValue;
	}

	std::string Array::toLua() const
	{
		std::string str("{");
		bool first = true;
		for (auto iter = mValue.begin(); iter != mValue.end(); ++iter)
		{
			if (!first)
			{
				str += ", ";
			}
			first = false;
			str += (*iter)->toLua();
		}
		str += "}";
		return str;
	}

}
}
}
