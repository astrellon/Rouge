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

	const char TileType_tableName[] = "am_game_TileType";

	int TileType_ctor(lua_State *lua);
	void TileType_wrap(lua_State *lua, am::game::TileType *stats);

	int TileType_dtor(lua_State *lua);
	int TileType_register(lua_State *lua);

	int TileType_get_name(lua_State *lua);

	int TileType_get_full_name(lua_State *lua);
	int TileType_set_full_name(lua_State *lua);

	int TileType_add_tile_type(lua_State *lua);
	int TileType_get_tile_type(lua_State *lua);

	am::game::TileType *Check_TileType(lua_State *lua, int n);

}
}
}
