#pragma once

#include "idata.h"

#include <string>

namespace am {
namespace util {
namespace data {

	class String : public IData 
	{
	public:

		String();
		String(const char *v);
		String(const String &copy);
		~String();

		virtual int type() const
		{
			return String::TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		virtual IData *clone() const;

		void value(const char *v);
		const char *value() const;

		virtual std::string toLua() const;

		static String *checkDataType(IData *data, const char *className);

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		std::string mValue;
	};

}
}
}
