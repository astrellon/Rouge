#include "lua_store.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/game.h>
#include <game/engine.h>
#include <game/store.h>
using namespace am::game;

#include <sstream>

#include <log/logger.h>

#include "lua_store.h"

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * The store class represents a collection of inventories that are owned
	 * by a character for the purpose of trade.
	 */
	/**
	 * Creates a new store instance.
	 */
	int Store_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (args == 1 && !lua_isstr(lua, 1))
		{
			return LuaState::expectedArgs(lua, "@new", 2, "", "string id");
		}
		if (args == 0)
		{
			Store *store = new Store();
			wrapRefObject<Store>(lua, store);
			lua_pushboolean(lua, true);
			return 2;
		}

		bool newStore = false;
		const char *id = lua_tostring(lua, 1);
		Game *game = Engine::getGame();
		base::Handle<Store> store(dynamic_cast<Store *>(game->getStore(id)));
		if (!store)
		{
			newStore = true;
			store = new Store();
			store->setStoreId(id);
		}
		wrapRefObject<Store>(lua, store);
		lua_pushboolean(lua, newStore);
		return 2;
	}
	/**
	 * Releases the reference counter on this store.
	 */
	int Store_dtor(lua_State *lua)
	{
		Store *inv = castUData<Store>(lua, 1);
		if (inv)
		{
			inv->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.store");
	}
	/**
	 * Compares this store with another store object.
	 * Will not return true for equivalent stores.
	 * @param am.store rhs The other store to compare with.
	 * @returns boolean True if the stores are the same object.
	 */
	int Store_eq(lua_State *lua)
	{
		Store *lhs = castUData<Store>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.store");
		}
		Store *rhs = castUData<Store>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}
	
	int Store_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Store_ctor },
			{ "__gc",  Store_dtor },
			{ "__eq", Store_eq },
			{ "owner", Store_owner },
			{ "store_id", Store_store_id },
			{ "buy", Store_buy },
			{ "sell", Store_sell },
			{ "create_inventory", Store_create_inventory },
			{ "add_inventory", Store_add_inventory },
			{ "remove_inventory", Store_remove_inventory },
			{ "remove_all_inventories", Store_remove_all_inventories },
			{ "inventory", Store_inventory },
			{ "inventories", Store_inventories },
			{ "num_inventories", Store_num_inventories },
			{ "default_inventory_size", Store_default_inventory_size },

			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Store::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the stores owner.
	 *
	 * @returns am.character The owner.
	 */
	/**
	 * Sets the store owner, can be nil.
	 *
	 * @param am.character owner The new owner.
	 * @returns am.store This
	 */
	int Store_owner(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			if (lua_gettop(lua) == 1)
			{
				Character *owner = store->getStoreOwner();
				if (owner)
				{
					wrapRefObject<Character>(lua, owner);
				}
				else
				{
					lua_pushnil(lua);
				}
				return 1;
			}
			else if (lua_isnil(lua, 2))
			{
				store->setStoreOwner(nullptr);
				lua_first(lua);
			}
			Character *owner = castUData<Character>(lua, 2);
			if (owner)
			{
				store->setStoreOwner(owner);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "owner", 2, "am.character owner", "nil owner");
		}
		return LuaState::expectedContext(lua, "owner", "am.store");
	}

	/**
	 * Returns the store's id. Used to uniquely identify the store.
	 *
	 * @returns string The store's id.
	 */
	int Store_store_id(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			lua_pushstring(lua, store->getStoreId());
			return 1;
		}
		return LuaState::expectedContext(lua, "store_id", "am.store");
	}

	/**
	 * Attempts to buy the item from the store using the purse from the buyer character.
	 *
	 * @param am.character buyer The character buying the item.
	 * @param am.item item The item to buy.
	 * @param number [1.0] value_factor A multiplier that affects the cost of the item.
	 * @returns am.return_code Return codes for the purchase:
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The item was bought.</td></tr>
	 *  <tr><td>nil_parameter</td><td>The buyer or item was nil.</td></tr>
	 *  <tr><td>no_store_owner</td><td>Cannot buy the item with no store owner.</td></tr>
	 *  <tr><td>no_inventories</td><td>Could not find the item in the store because there are no inventories.</td></tr>
	 *  <tr><td>item_not_in_inventory</td><td>Could not find the item in the store inventories.</td></tr>
	 *  <tr><td>not_enough_coin</td><td>Buyer does not have enough coin to buy the item.</td></tr>
	 * </table>
	 */
	int Store_buy(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			Character *obj = castUData<Character>(lua, 2);
			Item *item = castUData<Item>(lua, 3);
			if (obj && item)
			{
				float factor = 1.0f;
				if (lua_isnum(lua, 4))
				{
					factor = lua_tonumber(lua, 4);
				}
				lua_pushinteger(lua, store->buyItem(obj, item, factor));
				return 1;
			}
			return LuaState::expectedArgs(lua, "buy", "am.character buyer, am.item item, number [1.0] value_factor");
		}
		return LuaState::expectedContext(lua, "buy", "am.store");
	}

	/**
	 * Attempts to sell the item from the seller character.
	 * Has options for allowing the store to automatically expand by making
	 * new inventories if need be and allowing for the store owner to sell the item
	 * without actually having the coin to pay for the item.
	 *
	 * @param am.character seller The character selling the item.
	 * @param am.item item The item to sell.
	 * @param number [1.0] value_factor A multiplier that affects the cost of the item.
	 * @param boolean [false] make_new_inventory If there is not enough space in the inventories then a new one is made.
	 * @param boolean [false] allow_zero_coin As coin is taken from the store owner to give to the seller, this can force
	 *  the seller to receive the full amount even if the store owner doesn't have the available coin.
	 * @returns am.return_code Return codes for the sale:
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The item was sold.</td></tr>
	 *  <tr><td>nil_parameter</td><td>The seller or item was nil.</td></tr>
	 *  <tr><td>no_store_owner</td><td>Cannot sell the item with no store owner.</td></tr>
	 *  <tr><td>no_inventories</td><td>No inventories and make_new_inventory is false.</td></tr>
	 *  <tr><td>not_enough_inventory_space</td><td>Not enough space in the inventories to fit the item and make_new_inventory is false.</td></tr>
	 *  <tr><td>not_enough_coin</td><td>Store owner does not have enough coin to buy the item and allow_zero_coin is false.</td></tr>
	 * </table>
	 */
	int Store_sell(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			Character *obj = castUData<Character>(lua, 2);
			Item *item = castUData<Item>(lua, 3);
			if (obj && item)
			{
				float factor = 1.0f;
				if (lua_isnum(lua, 4))
				{
					factor = lua_tonumber(lua, 4);
				}
				bool makeNewInventory = false;
				if (lua_isbool(lua, 5))
				{
					makeNewInventory = lua_tobool(lua, 5);
				}
				bool allowZeroCoin = false;
				if (lua_isbool(lua, 6))
				{
					allowZeroCoin = lua_tobool(lua, 6);
				}
				lua_pushinteger(lua, store->sellItem(obj, item, factor, makeNewInventory, allowZeroCoin));
				return 1;
			}
			return LuaState::expectedArgs(lua, "sell", "am.character seller, am.item item, number [1.0] value_factor, boolean [false] make_new_inventory, boolean [false] allow_zero_coin");
		}
		return LuaState::expectedContext(lua, "sell", "am.store");
	}

	/**
	 * Creates a new store inventory using the default inventory size.
	 *
	 * @returns am.inventory The new store inventory.
	 */
	int Store_create_inventory(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			wrapRefObject<Inventory>(lua, store->createStoreInventory());
			return 1;
		}
		return LuaState::expectedContext(lua, "create_inventory", "am.store");
	}

	/**
	 * Adds the given inventory to the ones controlled by the store.
	 *
	 * @param am.inventory inventory The inventory to add.
	 * @returns boolean True if the inventory was successfully added, false if the inventory was nil or already part of the store.
	 */
	int Store_add_inventory(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			Inventory *inv = castUData<Inventory>(lua, 2);
			if (inv)
			{
				lua_pushboolean(lua, store->addStoreInventory(inv));
				return 1;
			}
			return LuaState::expectedArgs(lua, "add_inventory", "am.inventory inventory");
		}
		return LuaState::expectedContext(lua, "add_inventory", "am.store");
	}

	/**
	 * Removes the given inventory from the store.
	 *
	 * @param am.inventory inventory The inventory to remove.
	 * @returns boolean True if the inventory was successfully removed, false if the inventory was nil or not part of the store.
	 */
	int Store_remove_inventory(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			Inventory *inv = castUData<Inventory>(lua, 2);
			if (inv)
			{
				lua_pushboolean(lua, store->removeStoreInventory(inv));
				return 1;
			}
			return LuaState::expectedArgs(lua, "remove_inventory", "am.inventory inventory");
		}
		return LuaState::expectedContext(lua, "remove_inventory", "am.store");
	}

	/**
	 * Removes all the inventories from the store.
	 *
	 * @returns am.store This
	 */
	int Store_remove_all_inventories(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			store->clearAllStoreInventories();
		}
		return LuaState::expectedContext(lua, "remove_all_inventories", "am.store");
	}

	/**
	 * Returns the inventory at the given index, returns nil if outside of the number of inventories.
	 *
	 * @param integer index The index to get the inventory from.
	 * @returns am.inventory The inventory at the given index.
	 */
	int Store_inventory(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			if (lua_isnum(lua, 2))
			{
				int index = lua_tointeger(lua, 2);
				auto inventories = store->getStoreInventories();
				if (index < 0 || index >= static_cast<int>(inventories.size()))
				{
					std::stringstream ss;
					ss << "Unable to get inventory at index " << index << ", there are " << inventories.size() << " inventories in this store.";
					LuaState::warning(lua, ss.str().c_str());
					lua_pushnil(lua);
					return 1;
				}
				Inventory *inv = inventories[index];
				wrapRefObject<Inventory>(lua, inv);
				return 1;
			}
			return LuaState::expectedArgs(lua, "inventory", "integer index");
		}
		return LuaState::expectedContext(lua, "inventory", "am.store");
	}

	/**
	 * Returns an array of all the inventories in this store.
	 *
	 * @returns array All the inventories.
	 */
	int Store_inventories(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			auto inventories = store->getStoreInventories();

			lua_createtable(lua, inventories.size(), 0);
			for (size_t i = 0; i < inventories.size(); i++)
			{
				lua_pushinteger(lua, i);
				wrapRefObject<Inventory>(lua, inventories[i]);
				lua_settable(lua, -3);
			}
			return 1;
		}
		return LuaState::expectedContext(lua, "inventories", "am.store");
	}

	/**
	 * Returns an array of all the inventories in this store.
	 *
	 * @returns array All the inventories.
	 */
	int Store_num_inventories(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			auto inventories = store->getStoreInventories();
			lua_pushinteger(lua, inventories.size());
			return 1;
		}
		return LuaState::expectedContext(lua, "num_inventories", "am.store");
	}

	/**
	 * Returns an array of all the inventories in this store.
	 *
	 * @returns array All the inventories.
	 */
	int Store_default_inventory_size(lua_State *lua)
	{
		Store *store = castUData<Store>(lua, 1);
		if (store)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, store->getDefaultInventorySizeWidth());
				lua_pushinteger(lua, store->getDefaultInventorySizeHeight());
				return 2;
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				int width = lua_tointeger(lua, 2);
				int height = lua_tointeger(lua, 3);
				if (width < 0 || height < 0)
				{
					LuaState::warning(lua, "Width and/or height cannot be less than zero.");
					lua_first(lua);
				}
				store->setDefaultInventorySize(width, height);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "default_inventory_size", "integer width, integer height");
		}
		return LuaState::expectedContext(lua, "default_inventory_size", "am.store");
	}

}
}
}
