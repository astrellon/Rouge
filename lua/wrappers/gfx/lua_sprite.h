#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace gfx {

	int Sprite_ctor(lua_State *lua);
	int Sprite_dtor(lua_State *lua);
	int Sprite_eq(lua_State *lua);
	int Sprite_register(lua_State *lua);

	int Sprite_asset(lua_State *lua);

}
}
}
