#pragma once

#include "idata.h"

namespace am {
namespace util {
namespace data {

	class Boolean : public IData {
	public:

		Boolean();
		Boolean(bool v);
		Boolean(const Boolean &copy);
		~Boolean();

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		virtual IData *clone() const;

		void value(bool v);
		bool value() const;

		virtual std::string toLua() const;

		static Boolean *checkDataType(IData *data, const char *className);

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		bool mValue;
	};

}
}
}
