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
	/**
	 * @class
	 * The inventory class represents an inventory of items, it is not linked
	 * to a specific game object however a game object can be linked to an inventory.
	 * This allows for any character or item to hold an inventory.<br>
	 * An inventory has a size and as such the maximum number of items that can
	 * be stored will be limited by the size of each item.
	 */
	/**
	 * Creates a new inventory instance.
	 *
	 * @param integer width The width of the inventory in item size units.
	 * @param integer height The height of the inventory in item size units.
	 */
	int Inventory_ctor(lua_State *lua)
	{
		if (lua_isnum(lua, 1) && lua_isnum(lua, 2))
		{
			Inventory *inv = new Inventory(lua_tointeger(lua, 1), lua_tointeger(lua, 2));
			wrapRefObject<Inventory>(lua, inv);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "integer width, integer height");
	}
	/**
	 * Releases the reference counter on this inventory.
	 */
	int Inventory_dtor(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			inv->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.inventory");
	}
	/**
	 * Compares this inventory with another inventory object.
	 * Will not return true for equivalent inventories.
	 * @param am.inventory rhs The other inventory to compare with.
	 * @returns boolean True if the inventories are the same object.
	 */
	int Inventory_eq(lua_State *lua)
	{
		Inventory *lhs = castUData<Inventory>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.inventory");
		}
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
			{ "size", Inventory_size},
			{ "has_space_for", Inventory_has_space_for },
			{ "add_item", Inventory_add_item },
			{ "remove_item", Inventory_remove_item },
			{ "remove_all", Inventory_remove_all },
			{ "has_item", Inventory_has_item },
			{ "item_at", Inventory_item_at },
			{ "spots", Inventory_spots },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Inventory::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the width and height of this inventory.
	 *
	 * @returns integer The width of the inventory.
	 * @returns integer The height of the inventory.
	 */
	int Inventory_size(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			lua_pushinteger(lua, inv->getSpacesX());
			lua_pushinteger(lua, inv->getSpacesY());
			return 2;
		}
		return LuaState::expectedContext(lua, "size", "am.inventory");
	}
	/**
	 * Returns true if there is enough space to place the given item at
	 * the given location. False indicates that either the location will put 
	 * the item outside of the bounds of the inventory or that there is another
	 * item blocking this items placement.
	 *
	 * @param am.item item The item to place.
	 * @param integer x The x location to check at
	 * @param integer y The y location to check at
	 * @returns boolean True if there is space at the given location for the given item.
	 *  False otherwise if there isn't any space or the location is invalid.
	 */
	int Inventory_has_space_for(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				if (lua_isnum(lua, 3) && lua_isnum(lua, 4))
				{
					lua_pushboolean(lua, inv->hasSpaceFor(item, lua_tointeger(lua, 3), lua_tointeger(lua, 4)));
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "has_space_for", "am.item item, integer x, integer y");
		}
		return LuaState::expectedContext(lua, "has_space_for", "am.inventory");
	}
	/**
	 * Attempts to add an item to the inventory, returns true if a spot was
	 * found the item, false if the item was nil or if no spot could be found.
	 *
	 * @param am.item item The item to add to the inventory.
	 * @returns boolean True if the item was added to the inventory successfully.
	 */
	/**
	 * Attempts to add an item to the inventory at the given location, returns true
	 * if the space required for the item was available.
	 *
	 * @param am.item item The item to add to the inventory.
	 * @param integer x The x location to add the item at.
	 * @param integer y The y location to add the item at.
	 * @returns boolean True if the item was added to the inventory successfully.
	 */
	int Inventory_add_item(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				int args = lua_gettop(lua);
				if (args == 2)
				{
					lua_pushboolean(lua, inv->addItem(item));
					return 1;
				}
				else if (args == 4 && lua_isnum(lua, 3) && lua_isnum(lua, 4))
				{
					lua_pushboolean(lua, inv->addItem(item, lua_tointeger(lua, 3), lua_tointeger(lua, 4)));
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "add_item", 2, "am.item item", "am.item item, integer x, integer y");
		}
		return LuaState::expectedContext(lua, "add_item", "am.inventory");
	}
	/**
	 * Removes the given item from the inventory, returns true if the item was found in the inventory
	 * and removed.
	 *
	 * @param am.item item The item to remove from the inventory.
	 * @returns boolean True if the item was removed.
	 */
	int Inventory_remove_item(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				lua_pushboolean(lua, inv->removeItem(item));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_item", "am.item item");
		}
		return LuaState::expectedContext(lua, "remove_item", "am.inventory");
	}
	/**
	 * Removes all items from the inventory.
	 * 
	 * @returns boolean True if all items were successfully removed, false is there was an error.
	 */
	int Inventory_remove_all(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			lua_pushboolean(lua, inv->removeAll());
			return 1;
		}
		return LuaState::expectedContext(lua, "remove_all", "am.inventory");
	}
	/**
	 * Looks for the given item in the inventory. Returns true if the given item was found.
	 *
	 * @param am.item item The item to look for in the inventory.
	 * @returns boolean True if the item was found.
	 */
	int Inventory_has_item(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			Item *item = castUData<Item>(lua, 2);
			if (item)
			{
				lua_pushboolean(lua, inv->hasItem(item));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_item", "am.item item");
		}
		return LuaState::expectedContext(lua, "has_item", "am.inventory");
	}
	/**
	 * Returns the item at the given location, nil if there is no item at that location.
	 * Items do not overlap so there cannot be multiple items returned.
	 *
	 * @param integer locationX The x location to look at.
	 * @param integer locationY The y location to look at.
	 * @returns am.item The found item, or nil if it was nothing was found.
	 */
	int Inventory_item_at(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				wrapRefObject<Item>(lua, inv->getItemAt(lua_tointeger(lua, 2), lua_tointeger(lua, 3)));
				return 1;
			}
			return LuaState::expectedArgs(lua, "item_at", "integer x, integer y");
		}
		return LuaState::expectedContext(lua, "item_at", "am.inventory");
	}
	/**
	 * Returns an array of tables which represent all the items in the inventory.
	 * Each array element contains the item, and x and y locations.
	 * <pre>
	 * inv = am.inventory.new(3, 3)
	 * scroll1 = am.item.new()
	 * scroll1:inventory_size(2, 1)
	 * scroll2 = scroll1:clone()
	 * scroll1:name("Scroll 1")
	 * scroll2:name("Scroll 2")
	 * inv.add_item(scroll1)
	 * inv.add_item(scroll2)
	 *
	 * spots = inv:spots()
	 * for key, value in pairs(spots) do
	 *     am_log(key .. ": " .. value.item:name() .. " at " .. value.x .. ", " .. value.y)
	 * end
	 * 
	 * Outputs:
	 * 2: Scroll 2 at 0, 1
	 * 1: Scroll 1 at 0, 0
	 * </pre>
	 
	 * @returns Array Array of all the items in the inventory and their locations.
	 */
	int Inventory_spots(lua_State *lua)
	{
		Inventory *inv = castUData<Inventory>(lua, 1);
		if (inv)
		{
			LuaState L(lua);
			L.newTable();
			const Inventory::InventorySpots &spots = inv->getInventory();
			int i = 1;
			for (auto iter = spots.begin(); iter != spots.end(); ++iter)
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
		return LuaState::expectedContext(lua, "spots", "am.inventory");
	}

}
}
}
