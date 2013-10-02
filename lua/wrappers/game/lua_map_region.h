#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace game {

	int MapRegion_ctor(lua_State *lua);
	int MapRegion_dtor(lua_State *lua);
	int MapRegion_eq(lua_State *lua);

	int MapRegion_register(lua_State *lua);

}
}
}
