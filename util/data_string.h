#pragma once

#include "idata.h"

#include <string>

namespace am {
namespace util {
namespace data {

	class String : public IData {
	public:

		String();
		String(const char *v);
		~String();

		virtual int type() const
		{
			return String::TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		void value(const char *v);
		const char *value() const;

		virtual std::string toLua() const;

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		std::string mValue;
	};

}
}
}
