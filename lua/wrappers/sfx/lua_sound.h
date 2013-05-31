#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace sfx {
	class ISound;
}

namespace lua {
namespace sfx {

	int Sound_ctor(lua_State *lua);
	int Sound_dtor(lua_State *lua);
	int Sound_eq(lua_State *lua);
	int Sound_register(lua_State *lua);

	int Sound_filename(lua_State *lua);
	int Sound_streaming(lua_State *lua);

}
}
}
