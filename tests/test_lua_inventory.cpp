#include "test_lua_inventory.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/item.h>
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
		
		int loadResult = lua.loadString("Inventory = import(\"Inventory\")\n"
			"item = import(\"Item\")\n"
			"inv = Inventory.new(6, 4)\n"
			"function getSpace()\n"
			"	return inv:get_space()\n"
			"end\n"
			"function hasSpaceFor(item, x, y)\n"
			"	return inv:has_space_for(item, x, y)\n"
			"end\n"
			"function addItem(item, x, y)\n"
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
		lua.call(0, 2);
		equals(6, lua_tointeger(lua, -2));
		equals(4, lua_tointeger(lua, -1));
		lua.pop(2);

		Handle<Item> item(new Item());
		item->setInventorySize(2, 2);
		assert(lua.hasGlobalFunction("hasSpaceFor"));
		Item_wrap(lua, item);
		lua.push(0);
		lua.push(0);
		lua.call(3, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("hasSpaceFor"));
		Item_wrap(lua, item);
		lua.push(5);
		lua.push(0);
		lua.call(3, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		item->setInventorySize(7, 2);
		assert(lua.hasGlobalFunction("hasSpaceFor"));
		Item_wrap(lua, item);
		lua.push(0);
		lua.push(0);
		lua.call(3, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		item->setInventorySize(2, 2);
		assert(lua.hasGlobalFunction("addItem"));
		Item_wrap(lua, item);
		lua.call(1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addItem"));
		Item_wrap(lua, item);
		lua.call(1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeItem"));
		Item_wrap(lua, item);
		lua.call(1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeItem"));
		Item_wrap(lua, item);
		lua.call(1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		return true;
	}
}
}