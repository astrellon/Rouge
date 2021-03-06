#include "test_lua_item.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>

#include <game/item.h>
#include <game/engine.h>

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaItem::testSimple()
	{
		lua::LuaState lua;
		
		int loadResult = lua.loadString(
			"item = am.item.new()\n"
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
			//lua.call(0, 1);
			lua_acall(lua, 0, 1);
			base::Handle<game::Item> item(lua::castUData<game::Item>(lua, 1));

			assert(item != nullptr);

			am_equals(2, item->getReferenceCounter());

			assert(lua.hasGlobalFunction("setGameId"));
			//lua.call(0, 0);
			lua_acall(lua, 0, 0);

			am_equals(3, item->getReferenceCounter());
			
			lua.close();

			am_equals(2, item->getReferenceCounter());

		}
		
		return true;
	}

	bool TestLuaItem::testBodyPartTypes() 
	{
		lua::LuaState lua;
		
		int loadResult = lua.loadString(
			"item = am.item.new()\n"
			"function addPart(part)\n"
			"	item:add_body_type(part)\n"
			"end\n"
			"function removePart(part)\n"
			"	item:remove_body_type(part)\n"
			"end\n"
			"function hasPart(part)\n"
			"	return item:has_body_type(part)\n"
			"end\n"
			"function tableLength(T)\n"
			"	local count = 0\n"
			"	for _ in pairs(T) do count = count + 1 end\n"
			"	return count\n"
			"end\n"
			"function printParts()\n"
			"	parts = item:get_body_types()\n"
			"	am.debug.equals(2, tableLength(parts))\n"
			"	am.debug.equals(true, parts[\"arm\"])\n"
			"	am.debug.equals(true, parts[\"hand\"])\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		assert(lua.hasGlobalFunction("hasPart"));
		lua_pushstring(lua, "arm");
		lua_acall(lua, 1, 1);
		//lua.call(1, 1);
		am_equals(false, lua_tobool(lua, -1));
		lua_pop(lua, 1);

		assert(lua.hasGlobalFunction("addPart"));
		lua_pushstring(lua, "arm");
		//lua.call(1, 0);
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("hasPart"));
		lua_pushstring(lua, "arm");
		//lua.call(1, 1);
		lua_acall(lua, 1, 1);
		am_equals(true, lua_tobool(lua, -1));
		lua_pop(lua, 1);

		assert(lua.hasGlobalFunction("addPart"));
		lua_pushstring(lua, "hand");
		//lua.call(1, 0);
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("hasPart"));
		lua_pushstring(lua, "hand");
		//lua.call(1, 1);
		lua_acall(lua, 1, 1);
		am_equals(true, lua_tobool(lua, -1));
		lua_pop(lua, 1);

		assert(lua.hasGlobalFunction("printParts"));
		try 
		{
			//lua.call(0, 0);
			lua_acall(lua, 0, 0);
		}
		catch (...)
		{
			lua.logStack("ERR");
			return false;
		}

		return true;
	}
}
}