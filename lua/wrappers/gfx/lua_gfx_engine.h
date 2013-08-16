#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace gfx {

	int GfxEngine_register(lua_State *lua);

	int GfxEngine_asset(lua_State *lua);

}
}
}
