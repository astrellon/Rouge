#pragma once

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Number : public IData
	{
	public:

		Number();
		Number(int v);
		Number(unsigned int v);
		Number(double v);
		Number(const Number &copy);
		~Number();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		virtual IData *clone() const;

		void value(double v);
		void value(int v);
		double value() const;
		int valuei() const;

		template <class T>
		T value() const
		{
			return static_cast<T>(mValue);
		}

		virtual std::string toLua() const;

		static Number *checkDataType(IData *data, const char *className);

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		double mValue;
	};

}
}
}
