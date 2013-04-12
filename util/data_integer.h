#pragma once

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Integer : public IData {
	public:

		Integer();
		Integer(int v);
		~Integer();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		void value(int v);
		int value() const;

		virtual std::string toLua() const;

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		int mValue;
	};

}
}
}
