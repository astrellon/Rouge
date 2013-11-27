#include "idata.h"

#include "data_string.h"
#include "data_number.h"
#include "data_boolean.h"
#include "data_table.h"
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
		return nullptr;
	}
	int IData::integer() const
	{
		if (type() == Number::TYPE)
		{
			return math::round(dynamic_cast<const Number *>(this)->value());
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
			util::data::Table *table = new util::data::Table();
			lua.pushnil();
			while (lua_next(lua, -2) != 0)
			{
				int luaType = lua_type(lua, -2);
				if (luaType == LUA_TSTRING)
				{
					table->at(lua_tostring(lua, -2), fromLua(lua, -1));
				}
				else if (luaType == LUA_TNUMBER)
				{
					table->push(fromLua(lua, -1));
				}
				lua.pop(1);
			}
			return table;
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
			return nullptr;
		}
	}

}
}
}
