#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class TileType;
}

namespace lua {
namespace game {

	int TileType_ctor(lua_State *lua);
	int TileType_dtor(lua_State *lua);
	int TileType_eq(lua_State *lua);
	int TileType_register(lua_State *lua);

	int TileType_name(lua_State *lua);
	int TileType_full_name(lua_State *lua);

	int TileType_add_tile_type(lua_State *lua);
	int TileType_tile_type(lua_State *lua);

}
}
}
