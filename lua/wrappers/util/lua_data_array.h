#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace util {
namespace data {
	class Array;
}
}

namespace lua {
namespace util {
namespace data {

	int DataArray_ctor(lua_State *lua);
	int DataArray_dtor(lua_State *lua);
	int DataArray_eq(lua_State *lua);
	int DataArray_register(lua_State *lua);

	int DataArray_at(lua_State *lua);
	int DataArray_push(lua_State *lua);
	int DataArray_remove(lua_State *lua);
	int DataArray_len(lua_State *lua);

}
}
}
}
