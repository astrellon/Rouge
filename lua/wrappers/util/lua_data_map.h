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
	class Map;
}
}

namespace lua {
namespace util {
namespace data {

	int DataMap_ctor(lua_State *lua);
	int DataMap_dtor(lua_State *lua);
	int DataMap_eq(lua_State *lua);
	int DataMap_register(lua_State *lua);

	int DataMap_at(lua_State *lua);
	int DataMap_len(lua_State *lua);
	int DataMap_remove(lua_State *lua);

	int DataMap_each(lua_State *lua);

	bool pushData(lua_State *lua, am::util::data::IData *data);

}
}
}
}
