#include "test_lua_store.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>

#include <game/item.h>
#include <game/game.h>
#include <game/character.h>
#include <game/engine.h>

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaStore::testSimple()
	{
		lua::LuaState lua;
		
		int loadResult = lua.loadString(
			"store = am.store.new(\"store_1\")\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);
		
		return true;
	}

}
}
