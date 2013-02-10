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

	int Inventory_ctor(lua_State *lua);
	int Inventory_dtor(lua_State *lua);
	int Inventory_eq(lua_State *lua);
	int Inventory_register(lua_State *lua);

	int Inventory_size(lua_State *lua);
	
	int Inventory_has_space_for(lua_State *lua);
	int Inventory_add_item(lua_State *lua);
	int Inventory_remove_item(lua_State *lua);
	int Inventory_remove_all(lua_State *lua);
	int Inventory_has_item(lua_State *lua);
	int Inventory_item_at(lua_State *lua);

	int Inventory_spots(lua_State *lua);
}
}
}
