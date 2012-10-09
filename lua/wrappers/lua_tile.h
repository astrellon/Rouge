#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Tile;
	class TileType;
}

namespace lua {
namespace game {

	const char Tile_tableName[] = "am_game_Tile";

	int Tile_ctor(lua_State *lua);
	void Tile_wrap(lua_State *lua, am::game::Tile *stats);

	int Tile_dtor(lua_State *lua);
	int Tile_register(lua_State *lua);

	int Tile_get_name(lua_State *lua);
	int Tile_set_name(lua_State *lua);

	int Tile_get_full_name(lua_State *lua);
	int Tile_set_full_name(lua_State *lua);

	int Tile_get_description(lua_State *lua);
	int Tile_set_description(lua_State *lua);

	int Tile_add_tile_type(lua_State *lua);
	int Tile_remove_tile_type(lua_State *lua);
	int Tile_remove_all_tile_types(lua_State *lua);
	int Tile_has_tile_type(lua_State *lua);

	am::game::TileType *getTileType(lua_State *lua, int n);

	am::game::Tile *Check_Tile(lua_State *lua, int n);

}
}
}
