#pragma once

#include <vector>

#include <base/handle.h>
using namespace am::base;

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Array : public IData {
	public:

		typedef std::vector< Handle<IData> > Array_internal;
		typedef Array_internal::iterator iterator;
		typedef Array_internal::const_iterator const_iterator;

		Array();
		Array(const Array &copy);
		~Array();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		virtual IData *clone() const;

		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		void push(IData *v);
		void push(double v);
		void push(int v);
		void push(bool v);
		void push(const char *v);
		void push(const std::string &v);

		IData *operator[](unsigned int index);

		IData *at(unsigned int index) const;
		void remove(unsigned int index);

		template <class T>
		T *at(unsigned int index) const
		{
			if (index >= mValue.size())
			{
				throw std::out_of_range("Out of bounds");
			}
			return dynamic_cast<T *>(mValue[index].get());
		}

		Array_internal &inner();

		virtual void comment(const char *comment);
		virtual const char *comment() const;

		static Array *checkDataType(IData *data, const char *className, bool checkEmpty = true);

		virtual std::string toLua() const;

		static const int TYPE;
		static const char *TYPENAME;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Array_internal mValue;
		std::string mComment;
	};

}
}
}