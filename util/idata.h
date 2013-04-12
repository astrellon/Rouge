#pragma once

#include <string>

#include <base/imanaged.h>

namespace am {
namespace util {
namespace data {

	class IData : public am::base::IManaged {
	public:

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		const char *string() const;
		int integer() const;
		double number() const;
		bool boolean() const;

		virtual std::string toLua() const = 0;

		const static int TYPE;
		const static char *TYPENAME;

	};

}
}
}
