#include "idata.h"

#include "data_string.h"
//#include "data_integer.h"
#include "data_number.h"
#include "data_boolean.h"
#include "data_array.h"
#include "data_map.h"
#include "data_nil.h"

#include <math/math.h>

namespace am {
namespace util {
namespace data {

	const int IData::TYPE = 0;
	const char *IData::TYPENAME = "IData";

	IData *IData::NIL = new Nil();

	const char *IData::string() const
	{
		if (type() == String::TYPE)
		{
			return dynamic_cast<const String *>(this)->value();
		}
		return NULL;
	}
	int IData::integer() const
	{
		if (type() == Number::TYPE)
		{
			return am::math::round(dynamic_cast<const Number *>(this)->value());
		}
		return 0;
	}
	double IData::number() const
	{
		if (type() == Number::TYPE)
		{
			return dynamic_cast<const Number *>(this)->value();
		}
		return 0.0;
	}
	bool IData::boolean() const
	{
		if (type() == Boolean::TYPE)
		{
			return dynamic_cast<const Boolean *>(this)->value();
		}
		return false;
	}
	bool IData::isNil() const
	{
		return false;
	}

	IData *IData::fromLua(LuaState &lua, int n)
	{
		if (lua_istable(lua, n))
		{
			bool isArray = true;
			lua.pushnil();
			while (lua_next(lua, -2) != 0)
			{
				if (lua_type(lua, -2) == LUA_TSTRING)
				{
					isArray = false;
					lua.pop(1);
					lua.pop(1);
					break;
				}

				lua.pop(1);
			}

			if (isArray)
			{
				Array *arr = new Array();
				lua.pushnil();
				while (lua_next(lua, -2) != 0)
				{
					arr->push(fromLua(lua, -1));
					lua.pop(1);
				}
				return arr;
			}
			Map *map = new Map();
			lua.pushnil();
			while (lua_next(lua, -2) != 0)
			{
				map->push(lua_tostring(lua, -2), fromLua(lua, -1));
				lua.pop(1);
			}
			return map;
		}
		else
		{
			int type = lua_type(lua, n);
			if (type == LUA_TNUMBER)
			{
				return new Number(lua_tonumber(lua, n));
			}
			else if (type == LUA_TSTRING)
			{
				return new String(lua_tostring(lua, n));
			}
			else if (type == LUA_TBOOLEAN)
			{
				return new Boolean(lua_tobool(lua, n));
			}
			else if (type == LUA_TNIL)
			{
				return NIL;
			}
			return NULL;
		}
	}

}
}
}
