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

	int Item_set_item_type(lua_State *lua);
	int Item_get_item_type(lua_State *lua);

	int Item_set_inventory_size(lua_State *lua);
	int Item_get_inventory_size(lua_State *lua);

	int Item_set_item_location(lua_State *lua);
	int Item_get_item_location(lua_State *lua);

	int Item_set_quest_item_id(lua_State *lua);
	int Item_get_quest_item_id(lua_State *lua);
	int Item_is_quest_item(lua_State *lua);

	int Item_set_item_name(lua_State *lua);
	int Item_get_item_name(lua_State *lua);

	int Item_set_prefix(lua_State *lua);
	int Item_get_prefix(lua_State *lua);

	int Item_set_postfix(lua_State *lua);
	int Item_get_postfix(lua_State *lua);

	int Item_set_item_fullname(lua_State *lua);
	int Item_get_item_fullname(lua_State *lua);

	int Item_set_item_from(lua_State *lua);

	int Item_load_def(lua_State *lua);

	int Item_get_width(lua_State *lua);
	int Item_get_height(lua_State *lua);

	int Item_get_stat_modifiers(lua_State *lua);

	int Item_get_name(lua_State *lua);

	int Item_set_location(lua_State *lua);
	int Item_get_location(lua_State *lua);

	int Item_set_grid_location(lua_State *lua);
	int Item_get_grid_location(lua_State *lua);

	int Item_set_game_id(lua_State *lua);
	int Item_get_game_id(lua_State *lua);

	int Item_get_by_game_id(lua_State *lua);

}
}
}
