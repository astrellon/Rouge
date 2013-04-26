#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace util {
namespace data {
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

}
}
}
}
