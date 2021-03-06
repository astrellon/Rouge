#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Game;
	class GameObject;
}

namespace lua {
namespace game {

	int Game_ctor(lua_State *lua);
	int Game_dtor(lua_State *lua);
	int Game_eq(lua_State *lua);
	int Game_register(lua_State *lua);

	int Game_add_map(lua_State *lua);
	int Game_remove_map(lua_State *lua);
	int Game_remove_all_maps(lua_State *lua);

	int Game_map(lua_State *lua);
	int Game_has_map(lua_State *lua);
	int Game_current_map(lua_State *lua);

	int Game_add_game_object_to_map(lua_State *lua);
	int Game_remove_game_object_from_map(lua_State *lua);
	int Game_has_game_object_in_map(lua_State *lua);

	int Game_move_object_to_map(lua_State *lua);
	int Game_move_object_to_map_grid(lua_State *lua);

	int Game_main_character(lua_State *lua);

	int Game_get_game_object(lua_State *lua);
	int Game_register_game_object(lua_State *lua);
	int Game_deregister_game_object(lua_State *lua);

	int Game_char_def(lua_State *lua);
	int Game_item_def(lua_State *lua);

	int Game_add_dialogue(lua_State *lua);
	int Game_remove_dialogue(lua_State *lua);
	int Game_remove_all_dialogue(lua_State *lua);
	int Game_dialogue(lua_State *lua);
	int Game_available_dialogues(lua_State *lua);

	int Game_add_quest(lua_State *lua);
	int Game_remove_quest(lua_State *lua);
	int Game_quest(lua_State *lua);

	int Game_start_game(lua_State *lua);
	int Game_has_started(lua_State *lua);

	int Game_load_game(lua_State *lua);
	int Game_scenario_name(lua_State *lua);

	int Game_generic_dead_graphic(lua_State *lua);

	int Game_attrs(lua_State *lua);

	am::game::GameObject *getGameObject(lua_State *lua, int n);
	void wrapGameObject(lua_State *lua, am::game::GameObject *obj);
	bool moveGameObject(lua_State *lua, am::game::Game *game, bool grid);



}
}
}
