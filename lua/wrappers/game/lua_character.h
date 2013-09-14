#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace game {
	class Character;
}

namespace lua {
namespace game {

	int Character_ctor(lua_State *lua);
	int Character_dtor(lua_State *lua);
	int Character_eq(lua_State *lua);
	int Character_register(lua_State *lua);

	int Character_from_def(lua_State *lua);
	int Character_create(lua_State *lua);
	
	int Character_name(lua_State *lua);
	int Character_description(lua_State *lua);
	int Character_pickup_reach(lua_State *lua);

	int Character_stats(lua_State *lua);

	int Character_add_body_part(lua_State *lua);
	int Character_remove_body_part(lua_State *lua);
	int Character_has_body_part(lua_State *lua);
	int Character_body_parts(lua_State *lua);

	int Character_equip_item(lua_State *lua);
	int Character_unequip_item(lua_State *lua);
	int Character_equipped(lua_State *lua);

	int Character_inventory(lua_State *lua);
	
	int Character_pickup_item(lua_State *lua);
	int Character_add_item(lua_State *lua);
	int Character_remove_item(lua_State *lua);
	int Character_has_item(lua_State *lua);
	int Character_drop_item(lua_State *lua);

	int Character_age(lua_State *lua);
	int Character_race(lua_State *lua);
	int Character_gender(lua_State *lua);

	int Character_coin_purse(lua_State *lua);
	int Character_graphic(lua_State *lua);

	// GameObject methods
	int Character_location(lua_State *lua);
	int Character_grid_location(lua_State *lua);

	int Character_move(lua_State *lua);
	int Character_move_grid(lua_State *lua);

	int Character_talk_to(lua_State *lua);

	int Character_fixed_to_grid(lua_State *lua);
	int Character_map(lua_State *lua);
	int Character_original_map(lua_State *lua);

	int Character_add_passible_type(lua_State *lua);
	int Character_remove_passible_type(lua_State *lua);
	int Character_remove_all_passible_types(lua_State *lua);
	int Character_has_passible_type(lua_State *lua);
	int Character_get_passible_types(lua_State *lua);

	int Character_find(lua_State *lua);

	int Character_dialogue_component(lua_State *lua);
	int Character_game_id(lua_State *lua);

	int Character_add_event_listener(lua_State *lua);
	int Character_remove_event_listener(lua_State *lua);
	int Character_has_event_listener(lua_State *lua);

	int Character_experience(lua_State *lua);
	int Character_add_experience(lua_State *lua);
	int Character_level(lua_State *lua);
	int Character_add_level(lua_State *lua);
	int Character_max_level(lua_State *lua);

	int Character_debug(lua_State *lua);

	int Character_attrs(lua_State *lua);

}
}
}
