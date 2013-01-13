#include "lua_inventory.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/inventory.h>
using namespace am::game;

#include <sstream>
using namespace std;

#include <log/logger.h>

#include "lua_item.h"

namespace am {
namespace lua {
namespace game {

	int Inventory_ctor(lua_State *lua)
	{
		if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			Inventory *inv = new Inventory(lua_tointeger(lua, -2), lua_tointeger(lua, -1));
			wrapRefObject<Inventory>(lua, inv);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int Inventory_dtor(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			inv->release();
		}
		return 0;
	}

	int Inventory_eq(lua_State *lua)
	{
		Inventory *lhs = castUData<Inventory>(lua, 1);
		Inventory *rhs = castUData<Inventory>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Inventory_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Inventory_ctor },
			{ "__gc",  Inventory_dtor },
			{ "__eq", Inventory_eq },
			{ "get_space", Inventory_get_space },
			{ "has_space_for", Inventory_has_space_for },
			{ "add_item", Inventory_add_item },
			{ "remove_item", Inventory_remove_item },
			{ "remove_all", Inventory_remove_all },
			{ "has_item", Inventory_has_item },
			{ "get_item_at", Inventory_get_item_at },
			{ "get_spots", Inventory_get_spots },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Inventory::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	int Inventory_get_space(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			lua_pushinteger(lua, inv->getSpacesX());
			lua_pushinteger(lua, inv->getSpacesY());
			return 2;
		}
		lua_pushnil(lua);
		lua_pushnil(lua);
		return 2;
	}

	int Inventory_has_space_for(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (inv && item && lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
		{
			lua_pushboolean(lua, inv->hasSpaceFor(item, lua_tointeger(lua, -2), lua_tointeger(lua, -1)));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Inventory_add_item(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (inv && item)
		{
			if (lua_isnumber(lua, -2) && lua_isnumber(lua, -1))
			{
				lua_pushboolean(lua, inv->addItem(item, lua_tointeger(lua, -2), lua_tointeger(lua, -1)));
			}
			else
			{
				lua_pushboolean(lua, inv->addItem(item));
			}
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Inventory_remove_item(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (inv && item)
		{
			lua_pushboolean(lua, inv->removeItem(item));
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Inventory_remove_all(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			lua_pushboolean(lua, inv->removeAll());
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Inventory_has_item(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		Item *item = castUData<Item>(lua, 2);
		if (inv && item)
		{
			lua_pushboolean(lua, inv->hasItem(item));
			return 1;
		}
		lua_pushboolean(lua, false);
		return 1;
	}
	int Inventory_get_item_at(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			wrapRefObject<Item>(lua, inv->getItemAt(lua_tointeger(lua, -2), lua_tointeger(lua, -1)));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int Inventory_get_spots(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			LuaState L(lua);
			L.newTable();
			const Inventory::InventorySpots &spots = inv->getInventory();
			Inventory::InventorySpots::const_iterator iter;
			int i = 1;
			for (iter = spots.begin(); iter != spots.end(); ++iter)
			{
				L.newTable();
				lua_pushstring(lua, "item");
				wrapRefObject<Item>(lua, iter->getItem());
				lua_settable(lua, -3);
				L.setTableValue("x", iter->getX());
				L.setTableValue("y", iter->getY());
				lua_rawseti(lua, -2, i++);
			}

			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
