#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Game;
}

namespace lua {
namespace game {

	const char Game_tableName[] = "am_game_Game";

	int Game_ctor(lua_State *lua);
	void Game_wrap(lua_State *lua, am::game::Game *game);
	int Game_dtor(lua_State *lua);
	int Game_register(lua_State *lua);

	int Game_get_map(lua_State *lua);
	int Game_get_current_map(lua_State *lua);
	int Game_set_current_map(lua_State *lua);

	int Game_add_game_object(lua_State *lua);
	int Game_remove_game_object(lua_State *lua);
	int Game_has_game_object(lua_State *lua);

	int Game_move_object_to_map(lua_State *lua);
	int Game_move_object_to_map_grid(lua_State *lua);

	int Game_get_main_character(lua_State *lua);
	int Game_set_main_character(lua_State *lua);

	am::game::Game *Check_Game(lua_State *lua, int n);

}
}
}
