#include "data_map.h"

#include "data_boolean.h"
#include "data_number.h"
#include "data_string.h"
#include "data_integer.h"

namespace am {
namespace util {
namespace data {

	int Map::sDepth = 0;

	const int Map::TYPE = 6;
	const char *Map::TYPENAME = "Map";

	Map::Map()
	{
	}
	Map::~Map()
	{
	}

	Map::iterator Map::begin()
	{
		return mValue.begin();
	}
	Map::iterator Map::end()
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
		mValue[key] = new Integer(v);
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

	IData *Map::operator[](std::string key)
	{
		return mValue[key];
	}

	Map::Map_internal &Map::inner()
	{
		return mValue;
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
		auto cmtIter = mValue.find("__comment");
		Handle<data::IData> comment;
		
		if (cmtIter != mValue.end() && cmtIter->second->type() == data::String::TYPE)
		{
			comment = cmtIter->second;
			str += tabsLong;
			str += "--[[ ";
			str += cmtIter->second->string();
			str += " --]]\n";
			--size;
		}
		size_t i = 0;
		for (auto iter = mValue.begin(); iter != mValue.end(); ++iter)
		{
			if (cmtIter == iter)
			{
				continue;
			}
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

}
}
}
