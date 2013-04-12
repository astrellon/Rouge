#include "idata.h"

#include "data_string.h"
#include "data_integer.h"
#include "data_number.h"
#include "data_boolean.h"
#include "data_array.h"
#include "data_map.h"

namespace am {
namespace util {
namespace data {

	const int IData::TYPE = 0;
	const char *IData::TYPENAME = "IData";

	const char *IData::string() const
	{
		if (type() == String::TYPE)
		{
			return dynamic_cast<const String *>(this)->value();
		}
		return NULL;
	}
	int IData::integer() const
	{
		if (type() == Integer::TYPE)
		{
			return dynamic_cast<const Integer *>(this)->value();
		}
		return 0;
	}
	double IData::number() const
	{
		if (type() == Number::TYPE)
		{
			return dynamic_cast<const Number *>(this)->value();
		}
		return 0;
	}
	bool IData::boolean() const
	{
		if (type() == Boolean::TYPE)
		{
			return dynamic_cast<const Boolean *>(this)->value();
		}
		return false;
	}

}
}
}
