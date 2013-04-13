#pragma once

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Number : public IData {
	public:

		Number();
		Number(int v);
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
		void value(int v);
		double value() const;
		int valuei() const;

		virtual std::string toLua() const;

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		double mValue;
	};

}
}
}
