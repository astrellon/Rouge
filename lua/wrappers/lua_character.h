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

	const char Character_tableName[] = "am_game_Character";

	int Character_ctor(lua_State *lua);
	void Character_wrap(lua_State *lua, am::game::Character *character);
	int Character_dtor(lua_State *lua);
	int Character_register(lua_State *lua);
	
	int Character_get_name(lua_State *lua);
	int Character_set_name(lua_State *lua);

	int Character_set_pickup_reach(lua_State *lua);
	int Character_get_pickup_reach(lua_State *lua);

	int Character_get_stats(lua_State *lua);

	int Character_add_body_part(lua_State *lua);
	int Character_remove_body_part(lua_State *lua);
	int Character_has_body_part(lua_State *lua);

	int Character_equip_item(lua_State *lua);
	int Character_unequip_item(lua_State *lua);
	int Character_get_equipped(lua_State *lua);

	int Character_get_inventory(lua_State *lua);
	
	int Character_pickup_item(lua_State *lua);
	int Character_add_item(lua_State *lua);
	int Character_remove_item(lua_State *lua);
	int Character_has_item(lua_State *lua);
	int Character_drop_item(lua_State *lua);

	int Character_set_age(lua_State *lua);
	int Character_get_age(lua_State *lua);

	int Character_set_race(lua_State *lua);
	int Character_get_race(lua_State *lua);
	
	int Character_set_gender(lua_State *lua);
	int Character_get_gender(lua_State *lua);

	int Character_set_location(lua_State *lua);
	int Character_get_location(lua_State *lua);

	int Character_set_grid_location(lua_State *lua);
	int Character_get_grid_location(lua_State *lua);

	int Character_move(lua_State *lua);
	int Character_move_grid(lua_State *lua);

	int Character_talk_to(lua_State *lua);

	int Character_set_fixed_to_grid(lua_State *lua);
	int Character_is_fixed_to_grid(lua_State *lua);

	am::game::Character *Check_Character(lua_State *lua, int n);

}
}
}
