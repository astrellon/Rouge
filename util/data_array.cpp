#include "data_array.h"

#include <sstream>

#include <log/logger.h>

#include "data_boolean.h"
#include "data_number.h"
#include "data_string.h"
#include "data_map.h"

namespace am {
namespace util {
namespace data {

	const int Array::TYPE = 4;
	const char *Array::TYPENAME = "Array";

	const int Array::LUA_ID = 0x18;
	const char *Array::LUA_TABLENAME = "am_util_data_Array";

	Array::Array()
	{
	}
	Array::Array(const Array &copy) :
		mComment(copy.mComment)
	{
		for (auto iter = copy.begin(); iter != copy.end(); ++iter)
		{
			mValue.push_back((*iter)->clone());
		}
	}
	Array::~Array()
	{
	}

	IData *Array::clone() const
	{
		return new Array(*this);
	}

	Array::iterator Array::begin()
	{
		return mValue.begin();
	}
	Array::const_iterator Array::begin() const
	{
		return mValue.begin();
	}
	Array::iterator Array::end()
	{
		return mValue.end();
	}
	Array::const_iterator Array::end() const
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

	void Array::remove(unsigned int index)
	{
		mValue.erase(mValue.begin() + index);
	}
	IData *Array::operator[](unsigned int index)
	{
		return mValue[index];
	}
	IData *Array::at(unsigned int index) const
	{
		return mValue[index];
	}

	void Array::comment(const char *comment)
	{
		if (comment == NULL)
		{
			mComment.clear();
		}
		else
		{
			mComment = comment;
		}
	}
	const char *Array::comment() const
	{
		return mComment.c_str();
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

	Array *Array::checkDataType(IData *data, const char *className, bool checkEmpty)
	{
		if (checkEmpty)
		{
			Handle<Array> arr(dynamic_cast<Array *>(data));
			if (!arr)
			{
				Handle<Map> map(dynamic_cast<Map *>(data));
				if (map)
				{
					if (map->inner().size() > 0)
					{
						IData::logCheckErrorMessage<Array>(map->typeName(), className);
					}
				}
			}
			return arr;
		}
		return IData::checkDataType<Array>(data, className);
	}

}
}
}
