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
		
		game::Game *game = new game::Game();
		game::Engine::getEngine()->setCurrentGame(game);

		int loadResult = lua.loadString(
			"store = am.store.new(\"store_1\")\n"
			"owner = am.character.new(\"store_owner\")\n"
			"store:owner(owner)\n"
			"store:create_inventory()\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);
		
		game::Store *store = game->getStore("store_1");
		assert(store != nullptr);

		game::Character *owner = dynamic_cast<Character *>(game->getGameObject("store_owner"));
		assert(owner);
		assert(owner == store->getStoreOwner());

		am_equals(1, store->getNumStoreInventories());

		return true;
	}

}
}
