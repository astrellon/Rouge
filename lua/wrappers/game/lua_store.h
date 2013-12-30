#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace lua {
namespace game {

	int Store_ctor(lua_State *lua);
	int Store_dtor(lua_State *lua);
	int Store_eq(lua_State *lua);
	int Store_register(lua_State *lua);

	int Store_owner(lua_State *lua);
	int Store_store_id(lua_State *lua);
	int Store_buy(lua_State *lua);
	int Store_sell(lua_State *lua);
	int Store_create_inventory(lua_State *lua);
	int Store_add_inventory(lua_State *lua);
	int Store_remove_inventory(lua_State *lua);
	int Store_remove_all_inventories(lua_State *lua);
	int Store_inventory(lua_State *lua);
	int Store_inventories(lua_State *lua);
	int Store_num_inventories(lua_State *lua);
	int Store_default_inventory_size(lua_State *lua);
}
}
}

















