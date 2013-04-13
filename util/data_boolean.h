#pragma once

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Boolean : public IData {
	public:

		Boolean();
		Boolean(bool v);
		~Boolean();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		void value(bool v);
		bool value() const;

		virtual std::string toLua() const;

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		bool mValue;
	};

}
}
}
