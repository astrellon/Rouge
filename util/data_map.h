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

		Map();
		~Map();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		iterator begin();
		iterator end();

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

		virtual std::string toLua() const;

		static Map *checkDataType(IData *data, const char *className);

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		Map_internal mValue;

		static int sDepth;

		static void changeDepth(int delta);

		static void createTabs(std::string &tabs);
	};

}
}
}
