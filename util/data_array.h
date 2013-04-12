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

		Array();
		~Array();

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

		void push(IData *v);
		void push(double v);
		void push(int v);
		void push(bool v);
		void push(const char *v);
		void push(const std::string &v);

		IData *operator[](unsigned int index);

		template <class T>
		T *at(unsigned int index)
		{
			if (index >= mValue.size())
			{
				throw std::out_of_range("Out of bounds");
			}
			return dynamic_cast<T *>(mValue[index].get());
		}

		Array_internal &inner();

		std::string toLua() const;

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		Array_internal mValue;
	};

}
}
}
