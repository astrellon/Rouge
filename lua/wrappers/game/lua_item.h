#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Item;
}

namespace lua {
namespace game {

	int Item_ctor(lua_State *lua);
	int Item_dtor(lua_State *lua);
	int Item_eq(lua_State *lua);
	int Item_register(lua_State *lua);

	int Item_from_def(lua_State *lua);
	int Item_clone(lua_State *lua);

	int Item_graphic(lua_State *lua);
	int Item_ground_graphic(lua_State *lua);
	int Item_item_type(lua_State *lua);
	int Item_inventory_size(lua_State *lua);
	int Item_item_location(lua_State *lua);
	int Item_quest_item_id(lua_State *lua);
	int Item_is_quest_item(lua_State *lua);

	int Item_item_name(lua_State *lua);
	int Item_prefix(lua_State *lua);
	int Item_postfix(lua_State *lua);
	int Item_item_fullname(lua_State *lua);
	int Item_description(lua_State *lua);

	int Item_set_item_from(lua_State *lua);

	int Item_stat_modifiers(lua_State *lua);
	int Item_stat_modifiers_self(lua_State *lua);

	int Item_width(lua_State *lua);
	int Item_height(lua_State *lua);
	int Item_name(lua_State *lua);

	int Item_location(lua_State *lua);
	int Item_grid_location(lua_State *lua);

	int Item_game_id(lua_State *lua);

	int Item_add_event_listener(lua_State *lua);
	int Item_remove_event_listener(lua_State *lua);
	int Item_has_event_listener(lua_State *lua);

	int Item_find(lua_State *lua);

	int Item_attrs(lua_State *lua);

	int Item_add_body_type(lua_State *lua);
	int Item_remove_body_type(lua_State *lua);
	int Item_has_body_type(lua_State *lua);
	int Item_get_body_types(lua_State *lua);

}
}
}
