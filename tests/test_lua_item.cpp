#include "test_lua_item.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/item.h>
#include <game/engine.h>
using namespace am::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaItem::testSimple() {
		LuaState lua;
		
		int loadResult = lua.loadString("Item = import(\"Item\")\n"
			"item = Item.new()\n"
			"function setGameId()\n"
			"	item:game_id(\"testItem\")\n"
			"end\n"
			"function getItem()\n"
			"	return item\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);
		
		{
			assert(lua.hasGlobalFunction("getItem"));
			lua.call(0, 1);
			Handle<Item> item(castUData<Item>(lua, 1));

			assert(item != NULL);

			am_equals(2, item->getReferenceCounter());

			assert(lua.hasGlobalFunction("setGameId"));
			lua.call(0, 0);

			am_equals(3, item->getReferenceCounter());
			
			lua.close();

			am_equals(2, item->getReferenceCounter());

		}
		
		return true;
	}
}
}