#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Map;
}

namespace lua {
namespace game {

	int Map_ctor(lua_State *lua);
	int Map_dtor(lua_State *lua);
	int Map_eq(lua_State *lua);
	int Map_register(lua_State *lua);

	int Map_name(lua_State *lua);
	int Map_full_name(lua_State *lua);

	int Map_tile(lua_State *lua);
	int Map_tile_instance(lua_State *lua);
	int Map_tiles(lua_State *lua);

	int Map_map_size(lua_State *lua);

	int Map_add_game_object(lua_State *lua);
	int Map_remove_game_object(lua_State *lua);
	int Map_has_game_object(lua_State *lua);

	int Map_is_valid_location(lua_State *lua);
	int Map_is_valid_grid_location(lua_State *lua);

	int Map_load_def(lua_State *lua);

}
}
}
