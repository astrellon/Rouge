#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class TileInstance;
}

namespace lua {
namespace game {

	int TileInstance_ctor(lua_State *lua);
	int TileInstance_dtor(lua_State *lua);
	int TileInstance_eq(lua_State *lua);
	int TileInstance_register(lua_State *lua);

	int TileInstance_tile(lua_State *lua);
	int TileInstance_tile_frame(lua_State *lua);

}
}
}
