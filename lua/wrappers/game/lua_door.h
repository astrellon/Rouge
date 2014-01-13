#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace game {
	class Door;
}

namespace lua {
namespace game {

	int Door_ctor(lua_State *lua);
	int Door_dtor(lua_State *lua);
	int Door_eq(lua_State *lua);
	int Door_register(lua_State *lua);
	
	// Door methods
	int Door_tile_type(lua_State *lua);
	int Door_can_open_by(lua_State *lua);
	int Door_opened(lua_State *lua);
	int Door_lock(lua_State *lua);

	int Door_graphic(lua_State *lua);

	// GameObject methods
	int Door_name(lua_State *lua);
	int Door_description(lua_State *lua);

	int Door_location(lua_State *lua);
	int Door_grid_location(lua_State *lua);

	int Door_move(lua_State *lua);
	int Door_move_grid(lua_State *lua);
	int Door_talk_to(lua_State *lua);

	int Door_fixed_to_grid(lua_State *lua);
	int Door_map(lua_State *lua);
	int Door_original_map(lua_State *lua);

	int Door_add_passible_type(lua_State *lua);
	int Door_remove_passible_type(lua_State *lua);
	int Door_remove_all_passible_types(lua_State *lua);
	int Door_has_passible_type(lua_State *lua);
	int Door_get_passible_types(lua_State *lua);

	int Door_find(lua_State *lua);

	int Door_dialogue_component(lua_State *lua);
	int Door_game_id(lua_State *lua);

	int Door_add_event_listener(lua_State *lua);
	int Door_remove_event_listener(lua_State *lua);
	int Door_has_event_listener(lua_State *lua);

    int Door_interact_with(lua_State *lua);

	int Door_debug(lua_State *lua);

	int Door_attrs(lua_State *lua);

}
}
}
