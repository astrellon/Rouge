#pragma once

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Nil : public IData 
	{
	public:

		Nil();
		~Nil();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		virtual bool isNil() const;

		virtual IData *clone() const;

		virtual std::string toLua() const;

		static Nil *checkDataType(IData *data, const char *className);

		const static int TYPE;
		const static char *TYPENAME;

	};

}
}
}
