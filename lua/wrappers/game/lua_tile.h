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

	int Tile_ctor(lua_State *lua);
	int Tile_dtor(lua_State *lua);
	int Tile_eq(lua_State *lua);
	int Tile_register(lua_State *lua);

	int Tile_name(lua_State *lua);
	int Tile_full_name(lua_State *lua);
	int Tile_description(lua_State *lua);
	int Tile_tile_set(lua_State *lua);

	int Tile_add_tile_type(lua_State *lua);
	int Tile_remove_tile_type(lua_State *lua);
	int Tile_remove_all_tile_types(lua_State *lua);
	int Tile_has_tile_type(lua_State *lua);

	int Tile_tile_types(lua_State *lua);

	int Tile_load_def(lua_State *lua);

	am::game::TileType *getTileType(lua_State *lua, int n);
	
}
}
}
