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

	const char Engine_tableName[] = "am_game_Engine";

	int Engine_register(lua_State *lua);

	int Engine_set_current_game(lua_State *lua);
	int Engine_get_current_game(lua_State *lua);

	int Engine_set_grid_size(lua_State *lua);
	int Engine_get_grid_size(lua_State *lua);

	int Engine_using_tile_set(lua_State *lua);
	int Engine_clear_using_tile_set(lua_State *lua);

	int Engine_get_tile(lua_State *lua);
	
	int Engine_get_tile_set(lua_State *lua);
	int Engine_add_tile_set(lua_State *lua);
	int Engine_get_top_level_tile_set(lua_State *lua);

	int Engine_get_by_game_id(lua_State *lua);
	int Engine_register_game_object(lua_State *lua);
	int Engine_deregister_game_object(lua_State *lua);

	int Engine_add_race(lua_State *lua);
	int Engine_remove_race(lua_State *lua);
	int Engine_get_race(lua_State *lua);
	int Engine_get_unknown_race(lua_State *lua);

	int Engine_add_tile_type(lua_State *lua);
	int Engine_get_tile_type(lua_State *lua);

	// Static
	int Engine_get_game(lua_State *lua);
}
}
}
