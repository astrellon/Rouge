#pragma once

#include <map>
#include <string>

#include <base/handle.h>
using namespace am::base;

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Map : public IData {
	public:

		typedef std::map<std::string, Handle<IData> > Map_internal;
		typedef Map_internal::iterator iterator;
		typedef Map_internal::const_iterator const_iterator;

		Map();
		Map(const Map &copy);
		~Map();

		virtual IData *clone() const;

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		void push(const std::string &key, IData *v);
		void push(const std::string &key, double v);
		void push(const std::string &key, int v);
		void push(const std::string &key, bool v);
		void push(const std::string &key, const char *v);
		void push(const std::string &key, const std::string &v);

		IData *at(const std::string &key);

		template <class T>
		T *at(const std::string &key)
		{
			auto iter = mValue.find(key);
			if (iter == mValue.end())
			{
				return NULL;
			}
			return dynamic_cast<T *>(mValue[key].get());
		}

		Map_internal &inner();

		virtual void comment(const char *comment);
		virtual const char *comment() const;

		virtual std::string toLua() const;

		static Map *checkDataType(IData *data, const char *className, bool checkEmpty = true);

		const static int TYPE;
		const static char *TYPENAME;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Map_internal mValue;
		std::string mComment;

		static int sDepth;

		static void changeDepth(int delta);

		static void createTabs(std::string &tabs);
	};

}
}
}
