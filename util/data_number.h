#pragma once

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Number : public IData {
	public:

		Number();
		Number(double v);
		~Number();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		void value(double v);
		double value() const;

		virtual std::string toLua() const;

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		double mValue;
	};

}
}
}
