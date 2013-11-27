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

    class Number;
    class String;
    class Table;
    class Boolean;

	class IData : public base::IManaged {
	public:

		virtual int type() const
		{
			return TYPE;
		}
		virtual const char *typeName() const
		{
			return TYPENAME;
		}

		virtual IData *clone() const = 0;

		const char *string() const;
		int integer() const;
		double number() const;
		bool boolean() const;
		virtual bool isNil() const;

		template <class T>
		T number() const
		{
			return static_cast<T>(number());
		}

		template <class T>
		static T *checkDataType(IData *data, const char *className, bool logMessage = true)
		{
			T *result = dynamic_cast<T *>(data);
			if (!result)
			{
				logCheckErrorMessage<T>(data->typeName(), className);
				return nullptr;
			}
			return result;
		}

		virtual std::string toLua() const = 0;

		static IData *fromLua(LuaState &lua, int n);

		static IData *NIL;

		const static int TYPE;
		const static char *TYPENAME;

	protected:

		template <class T>
		static void logCheckErrorMessage(const char *dataType, const char *className)
		{
			std::stringstream ss;
			ss << "Unable to load " << className << " from '" << dataType
			   << "', must be a " << T::TYPENAME;
			am_log("LOADERR", ss);
		}

	};

}
}
}
