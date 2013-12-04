#include "data_table.h"

#include <sstream>

#include <log/logger.h>

#include "data_boolean.h"
#include "data_number.h"
#include "data_string.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace util {
namespace data {

	int Table::sDepth = 0;

	const int Table::TYPE = 8;
	const char *Table::TYPENAME = "Table";

	const int Table::LUA_ID = LUA_ID_TABLE;
	const char *Table::LUA_TABLENAME = LUA_TABLE_TABLE;

	Table::Table()
	{
	}
	Table::Table(const Table &copy) :
		mComment(copy.mComment)
	{
		for (auto iter = copy.beginMap(); iter != copy.endMap(); ++iter)
		{
			mMapValue[iter->first] = iter->second->clone();
		}
		for (auto iter = copy.beginArray(); iter != copy.endArray(); ++iter)
		{
			mArrayValue.push_back((*iter)->clone());
		}
	}
	Table::~Table()
	{
	}

	IData *Table::clone() const
	{
		return new Table(*this);
	}

	Table::map_iterator Table::beginMap()
	{
		return mMapValue.begin();
	}
	Table::map_const_iterator Table::beginMap() const
	{
		return mMapValue.begin();
	}
	Table::map_iterator Table::endMap()
	{
		return mMapValue.end();
	}
	Table::map_const_iterator Table::endMap() const
	{
		return mMapValue.end();
	}

	Table::array_iterator Table::beginArray()
	{
		return mArrayValue.begin();
	}
	Table::array_const_iterator Table::beginArray() const
	{
		return mArrayValue.begin();
	}
	Table::array_iterator Table::endArray()
	{
		return mArrayValue.end();
	}
	Table::array_const_iterator Table::endArray() const
	{
		return mArrayValue.end();
	}

	void Table::at(const std::string &key, IData *v)
	{
		mMapValue[key] = v;
	}
	void Table::at(const std::string &key, double v)
	{
		mMapValue[key] = new Number(v);
	}
	void Table::at(const std::string &key, int v)
	{
		mMapValue[key] = new Number(v);
	}
	void Table::at(const std::string &key, unsigned int v)
	{
		mMapValue[key] = new Number(v);
	}
	void Table::at(const std::string &key, bool v)
	{
		mMapValue[key] = new Boolean(v);
	}
	void Table::at(const std::string &key, const char *v)
	{
		mMapValue[key] = new String(v);
	}
	void Table::at(const std::string &key, const std::string &v)
	{
		mMapValue[key] = new String(v.c_str());
	}

	void Table::push(IData *v)
	{
		mArrayValue.push_back(v);
	}
	void Table::push(double v)
	{
		mArrayValue.push_back(new Number(v));
	}
	void Table::push(int v)
	{
		mArrayValue.push_back(new Number(v));
	}
	void Table::push(unsigned int v)
	{
		mArrayValue.push_back(new Number(v));
	}
	void Table::push(bool v)
	{
		mArrayValue.push_back(new Boolean(v));
	}
	void Table::push(const char *v)
	{
		mArrayValue.push_back(new String(v));
	}
	void Table::push(const std::string &v)
	{
		mArrayValue.push_back(new String(v.c_str()));
	}

	void Table::at(int index, IData *v)
	{
		fillArray(index);
		mArrayValue[index] = v;
	}
	void Table::at(int index, double v)
	{
		fillArray(index);
		mArrayValue[index] = new Number(v);
	}
	void Table::at(int index, int v)
	{
		fillArray(index);
		mArrayValue[index] = new Number(v);
	}
	void Table::at(int index, unsigned int v)
	{
		fillArray(index);
		mArrayValue[index] = new Number(v);
	}
	void Table::at(int index, bool v)
	{
		fillArray(index);
		mArrayValue[index] = new Boolean(v);
	}
	void Table::at(int index, const char *v)
	{
		fillArray(index);
		mArrayValue[index] = new String(v);
	}
	void Table::at(int index, const std::string &v)
	{
		fillArray(index);
		mArrayValue[index] = new String(v.c_str());
	}

	IData *Table::at(const std::string &key)
	{
		return mMapValue[key];
	}
	IData *Table::at(int index)
	{
		return mArrayValue[index];
	}

	Table::Map_internal &Table::mapInner()
	{
		return mMapValue;
	}
	Table::Array_internal &Table::arrayInner()
	{
		return mArrayValue;
	}

	void Table::remove(const char *key)
	{
		mMapValue.erase(key);
	}
	void Table::remove(int index)
	{
		mArrayValue.erase(mArrayValue.begin() + index);
	}

	std::string Table::toLua() const
	{
		size_t size = mMapValue.size() + mArrayValue.size();
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
		}
		size_t i = 0;
		for (auto iter = mArrayValue.begin(); iter != mArrayValue.end(); ++iter)
		{
			str += tabsLong;
			str += (*iter)->toLua();
			if (i++ < size - 1)
			{
				str += ",\n";
			}
		}
		for (auto iter = mMapValue.begin(); iter != mMapValue.end(); ++iter)
		{
			str += tabsLong;
			str += iter->first;
			str += " = ";
			str += iter->second->toLua();
			if (i++ < size - 1)
			{
				str += ",\n";
			}
		}

		changeDepth(-1);
		str += '\n';
		str += tabsShort;
		str += '}';
		return str;
	}

	void Table::comment(const char *comment)
	{
		if (comment == nullptr)
		{
			mComment.clear();
		}
		else
		{
			mComment = comment;
		}
	}
	const char *Table::comment() const
	{
		return mComment.c_str();
	}

	void Table::changeDepth(int delta)
	{
		sDepth += delta;
		if (sDepth < 0)
		{
			sDepth = 0;
		}
	}

	void Table::createTabs(std::string &tabs)
	{
		tabs.resize(sDepth);
		for (int i = 0; i < sDepth; i++)
		{
			tabs[i] = '\t';
		}
	}

	Table *Table::checkDataType(IData *data, const char *className)
	{
		return IData::checkDataType<Table>(data, className);
	}

	void Table::fillArray(int index)
	{
		while (static_cast<size_t>(index) >= mArrayValue.size())
		{
			mArrayValue.push_back(NIL);
		}
	}

}
}
}
