#include "test_lua_inventory.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/item.h>
#include <game/inventory.h>
using namespace am::game;

#include <lua/wrappers/game/lua_item.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaInventory::testSimple() {
		LuaState lua;
		
		int loadResult = lua.loadString(
			"inv = am.inventory.new(6, 4)\n"
			"function getSpace()\n"
			"	return inv:size()\n"
			"end\n"
			"function hasSpaceFor(item, x, y)\n"
			"	return inv:has_space_for(item, x, y)\n"
			"end\n"
			"function addItem(item)\n"
			"	return inv:add_item(item)\n"
			"end\n"
			"function addItemAt(item, x, y)\n"
			"	return inv:add_item(item, x, y)\n"
			"end\n"
			"function removeItem(item)\n"
			"	return inv:add_item(item)\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		assert(lua.hasGlobalFunction("getSpace"));
		lua_acall(lua, 0, 2);
		am_equals(6, lua_tointeger(lua, -2));
		am_equals(4, lua_tointeger(lua, -1));
		lua.pop(2);

		base::Handle<Item> item(new Item());
		item->setInventorySize(2, 2);
		assert(lua.hasGlobalFunction("hasSpaceFor"));
		wrapRefObject<Item>(lua, item);
		lua.push(0);
		lua.push(0);
		lua_acall(lua, 3, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("hasSpaceFor"));
		wrapRefObject<Item>(lua, item);
		lua.push(5);
		lua.push(0);
		lua_acall(lua, 3, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		item->setInventorySize(7, 2);
		assert(lua.hasGlobalFunction("hasSpaceFor"));
		wrapRefObject<Item>(lua, item);
		lua.push(0);
		lua.push(0);
		lua_acall(lua, 3, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		item->setInventorySize(2, 2);
		assert(lua.hasGlobalFunction("addItem"));
		wrapRefObject<Item>(lua, item);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addItem"));
		wrapRefObject<Item>(lua, item);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeItem"));
		wrapRefObject<Item>(lua, item);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeItem"));
		wrapRefObject<Item>(lua, item);
		lua_acall(lua, 1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaInventory::testSpots() {
		LuaState lua;
		
		int loadResult = lua.loadString(
			"inv = am.inventory.new(3, 3)\n"
			"scroll1 = am.item.new()\n"
			"scroll1:inventory_size(2, 1)\n"
			"scroll1:item_name(\"Scroll 1\")\n"
			"scroll2 = scroll1:clone()\n"
			"scroll2:item_name(\"Scroll 2\")\n"
			"inv:add_item(scroll1)\n"
			"inv:add_item(scroll2)\n"

			"spots = inv:spots()\n"
			"for key, value in pairs(spots) do\n"
			"    am.debug.log(key .. \": \" .. value.item:name() .. \" at \" .. value.x .. \", \" .. value.y)\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		return true;
	}

	bool TestLuaInventory::testEvents() {
		
		base::Handle<Inventory> inventory(new Inventory(6, 4));
		base::Handle<TestHandler> handler(new TestHandler());
		base::Handle<Item> testItem(new Item());
		testItem->setInventorySize(1, 1);
		inventory->addEventListener("inventory_add", handler);
		inventory->addEventListener("inventory_before_add", handler);
		inventory->addEventListener("inventory_remove", handler);
		inventory->addEventListener("inventory_before_remove", handler);

		am_equals(false, inventory->hasItem(testItem));
		am_equals(0, handler->counter);
		am_equals(true, inventory->addItem(testItem));
		am_equals(true, inventory->hasItem(testItem));
		am_equals(2, handler->counter);

		inventory->removeItem(testItem);
		am_equals(false, inventory->hasItem(testItem));
		am_equals(4, handler->counter);

		handler->accept = false;
		am_equals(false, inventory->addItem(testItem));
		am_equals(false, inventory->hasItem(testItem));
		am_equals(5, handler->counter);

		handler->accept = true;
		am_equals(true, inventory->addItem(testItem));
		am_equals(true, inventory->hasItem(testItem));
		am_equals(7, handler->counter);

		handler->accept = false;
		am_equals(false, inventory->removeItem(testItem));
		am_equals(true, inventory->hasItem(testItem));
		am_equals(8, handler->counter);

		return true;
	}

	TestLuaInventory::TestHandler::TestHandler() :
		accept(true),
		counter(0)
	{
	}
	void TestLuaInventory::TestHandler::onEvent(InventoryEvent *e)
	{
		counter++;
		if (!accept)
		{
			e->stopPropagation();
		}
	}

}
}
