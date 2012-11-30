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
			"item:set_game_id(\"testItem\")\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);
		
		{
			Handle<Item> item(dynamic_cast<Item *>(Engine::getEngine()->getByGameId("testItem")));
			assert(item != NULL);

			equals(2, item->getReferenceCounter());
			
			lua.close();

			equals(1, item->getReferenceCounter());

		}
		
		return true;
	}
}
}