#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

#define IS_LUA_PRIMATIVE(type) type == LUA_TNUMBER || type == LUA_TBOOLEAN || \
	type == LUA_TSTRING || type == LUA_TTABLE || \
	type == LUA_TNIL

namespace am {
namespace util {
namespace data {
	class IData;
	class Table;
}
}

namespace lua {
namespace util {
namespace data {

	int DataTable_ctor(lua_State *lua);
	int DataTable_dtor(lua_State *lua);
	int DataTable_eq(lua_State *lua);
	int DataTable_register(lua_State *lua);

	int DataTable_at(lua_State *lua);
	int DataTable_len(lua_State *lua);
	int DataTable_remove(lua_State *lua);
	int DataTable_each(lua_State *lua);

	bool pushData(lua_State *lua, am::util::data::IData *data);

}
}
}
}
