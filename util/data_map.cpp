#include "data_map.h"

#include <sstream>

#include <log/logger.h>

#include "data_boolean.h"
#include "data_number.h"
#include "data_array.h"
#include "data_string.h"

namespace am {
namespace util {
namespace data {

	int Map::sDepth = 0;

	const int Map::TYPE = 6;
	const char *Map::TYPENAME = "Map";

	const int Map::LUA_ID = 0x17;
	const char *Map::LUA_TABLENAME = "am_util_data_Map";

	Map::Map()
	{
	}
	Map::Map(const Map &copy) :
		mComment(copy.mComment)
	{
		for (auto iter = copy.begin(); iter != copy.end(); ++iter)
		{
			mValue[iter->first] = iter->second->clone();
		}
	}
	Map::~Map()
	{
	}

	IData *Map::clone() const
	{
		return new Map(*this);
	}

	Map::iterator Map::begin()
	{
		return mValue.begin();
	}
	Map::const_iterator Map::begin() const
	{
		return mValue.begin();
	}
	Map::iterator Map::end()
	{
		return mValue.end();
	}
	Map::const_iterator Map::end() const
	{
		return mValue.end();
	}

	void Map::push(const std::string &key, IData *v)
	{
		mValue[key] = v;
	}
	void Map::push(const std::string &key, double v)
	{
		mValue[key] = new Number(v);
	}
	void Map::push(const std::string &key, int v)
	{
		mValue[key] = new Number(v);
	}
	void Map::push(const std::string &key, bool v)
	{
		mValue[key] = new Boolean(v);
	}
	void Map::push(const std::string &key, const char *v)
	{
		mValue[key] = new String(v);
	}
	void Map::push(const std::string &key, const std::string &v)
	{
		mValue[key] = new String(v.c_str());
	}

	IData *Map::at(const std::string &key)
	{
		return mValue[key];
	}

	Map::Map_internal &Map::inner()
	{
		return mValue;
	}

	void Map::remove(const char *key)
	{
		mValue.erase(key);
	}

	std::string Map::toLua() const
	{
		size_t size = mValue.size();
		if (size == 0)
		{
			return "{}";
		}
		std::string str("{\n");

		std::string tabsShort;
		createTabs(tabsShort);
		changeDepth(1);
		std::string tabsLong(tabsShort);
		tabsLong += '\t';

		bool first = true;
		if (!mComment.empty())
		{
			str += tabsLong;
			str += "--[[ ";
			str += mComment;
			str += " --]]\n";
			--size;
		}
		size_t i = 0;
		for (auto iter = mValue.begin(); iter != mValue.end(); ++iter)
		{
			str += tabsLong;
			str += iter->first;
			str += " = ";
			str += iter->second->toLua();
			if (i < size - 1)
			{
				str += ",\n";
			}
			i++;
		}

		changeDepth(-1);
		str += '\n';
		str += tabsShort;
		str += '}';
		return str;
	}

	void Map::comment(const char *comment)
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
	const char *Map::comment() const
	{
		return mComment.c_str();
	}

	void Map::changeDepth(int delta)
	{
		sDepth += delta;
		if (sDepth < 0)
		{
			sDepth = 0;
		}
	}

	void Map::createTabs(std::string &tabs)
	{
		tabs.resize(sDepth);
		for (int i = 0; i < sDepth; i++)
		{
			tabs[i] = '\t';
		}
	}

	Map *Map::checkDataType(IData *data, const char *className, bool checkEmpty)
	{
		if (checkEmpty)
		{
			Handle<Map> map(dynamic_cast<Map *>(data));
			if (!map)
			{
				Handle<Array> arr(dynamic_cast<Array *>(data));
				if (arr)
				{
					if (arr->inner().size() > 0)
					{
						IData::logCheckErrorMessage<Map>(arr->typeName(), className);
					}
				}
			}
			return map;
		}
		return IData::checkDataType<Map>(data, className);
	}

}
}
}
