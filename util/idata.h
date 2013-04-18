#pragma once

#include <sstream>
#include <string>

#include <base/imanaged.h>

#include <log/logger.h>

#include <lua/lua_state.h>
using namespace am::lua;

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

		template <class T>
		T number() const
		{
			if (type() == Number::TYPE)
			{
				return dynamic_cast<const Number *>(this)->value<T>();
			}
			return static_cast<T>(0);
		}

		template <class T>
		static T *checkDataType(IData *data, const char *className)
		{
			T *result = dynamic_cast<T *>(data);
			if (!result)
			{
				std::stringstream ss;
				ss << "Unable to load " << className << " from '" << data->typeName()
					<< "', must be a " << T::TYPENAME;
				am_log("LOADERR", ss);
				return NULL;
			}
			return result;
		}

		virtual std::string toLua() const = 0;

		static IData *fromLua(LuaState &lua, int n);

		const static int TYPE;
		const static char *TYPENAME;

	};

}
}
}
