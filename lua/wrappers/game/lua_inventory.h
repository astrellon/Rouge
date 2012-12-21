#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Inventory;
}

namespace lua {
namespace game {

	const char Inventory_tableName[] = "am_game_Inventory";

	int Inventory_ctor(lua_State *lua);
	void Inventory_wrap(lua_State *lua, am::game::Inventory *inv);
	int Inventory_dtor(lua_State *lua);
	int Inventory_eq(lua_State *lua);
	int Inventory_register(lua_State *lua);

	am::game::Inventory *Check_Inventory(lua_State *lua, int n);

	int Inventory_get_space(lua_State *lua);
	
	int Inventory_has_space_for(lua_State *lua);
	int Inventory_add_item(lua_State *lua);
	int Inventory_remove_item(lua_State *lua);
	int Inventory_remove_all(lua_State *lua);
	int Inventory_has_item(lua_State *lua);
	int Inventory_get_item_at(lua_State *lua);

	int Inventory_get_spots(lua_State *lua);
}
}
}
