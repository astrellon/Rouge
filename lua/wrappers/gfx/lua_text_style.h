#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace gfx {

	int TextStyle_ctor(lua_State *lua);
	int TextStyle_dtor(lua_State *lua);
	int TextStyle_eq(lua_State *lua);
	int TextStyle_register(lua_State *lua);

}
}
}
