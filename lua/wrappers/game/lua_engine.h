#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Engine;
}

namespace lua {
namespace game {

	int Engine_register(lua_State *lua);

	int Engine_game(lua_State *lua);
	int Engine_grid_size(lua_State *lua);

	int Engine_using_tile_set(lua_State *lua);
	int Engine_clear_using_tile_set(lua_State *lua);

	int Engine_tile(lua_State *lua);
	
	int Engine_tile_set(lua_State *lua);
	int Engine_add_tile_set(lua_State *lua);
	int Engine_top_level_tile_set(lua_State *lua);

	int Engine_get_by_game_id(lua_State *lua);
	int Engine_register_game_object(lua_State *lua);
	int Engine_deregister_game_object(lua_State *lua);

	int Engine_add_race(lua_State *lua);
	int Engine_remove_race(lua_State *lua);
	int Engine_race(lua_State *lua);
	int Engine_unknown_race(lua_State *lua);

	int Engine_add_tile_type(lua_State *lua);
	int Engine_tile_type(lua_State *lua);

	int Engine_bgm(lua_State *lua);
}
}
}
