#pragma once

#include <map>
#include <string>
#include <vector>

#include <base/handle.h>

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Table : public IData {
	public:

		typedef std::map<std::string, base::Handle<IData> > Map_internal;
		typedef Map_internal::iterator map_iterator;
		typedef Map_internal::const_iterator map_const_iterator;

		typedef std::vector< base::Handle<IData> > Array_internal;
		typedef Array_internal::iterator array_iterator;
		typedef Array_internal::const_iterator array_const_iterator;

		Table();
		Table(const Table &copy);
		~Table();

		virtual IData *clone() const;

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		map_iterator beginMap();
		map_const_iterator beginMap() const;
		map_iterator endMap();
		map_const_iterator endMap() const;

		array_iterator beginArray();
		array_const_iterator beginArray() const;
		array_iterator endArray();
		array_const_iterator endArray() const;

		void at(const std::string &key, IData *v);
		void at(const std::string &key, double v);
		void at(const std::string &key, int v);
		void at(const std::string &key, unsigned int v);
		void at(const std::string &key, bool v);
		void at(const std::string &key, const char *v);
		void at(const std::string &key, const std::string &v);

		void at(int index, IData *v);
		void at(int index, double v);
		void at(int index, int v);
		void at(int index, unsigned int v);
		void at(int index, bool v);
		void at(int index, const char *v);
		void at(int index, const std::string &v);

		void push(IData *v);
		void push(double v);
		void push(int v);
		void push(unsigned int v);
		void push(bool v);
		void push(const char *v);
		void push(const std::string &v);

		IData *at(const std::string &key);
		IData *at(int index);

		template <class T>
		T *at(const std::string &key)
		{
			auto iter = mMapValue.find(key);
			if (iter == mMapValue.end())
			{
				return nullptr;
			}
			return dynamic_cast<T *>(mMapValue[key].get());
		}
		template <class T>
		T *at(int index)
		{
			if (index < 0 || index >= static_cast<int>(mArrayValue.size()))
			{
				return nullptr;
			}
			return dynamic_cast<T *>(mArrayValue[index].get());
		}

		Map_internal &mapInner();
		Array_internal &arrayInner();
		void remove(const char *key);
		void remove(int index);

		virtual void comment(const char *comment);
		virtual const char *comment() const;

		virtual std::string toLua() const;

		static Table *checkDataType(IData *data, const char *className);

		const static int TYPE;
		const static char *TYPENAME;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Map_internal mMapValue;
		Array_internal mArrayValue;
		std::string mComment;

		static int sDepth;

		static void changeDepth(int delta);

		static void createTabs(std::string &tabs);

		void fillArray(int index);
	};

}
}
}
