#include "test_lua_quest.h"

#include <tests/asserts.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/lua_quest.h>
using namespace am::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	int TestLuaQuest::sLevel = 1;

	int TestLuaQuest::getLevel(lua_State *lua)
	{
		lua_pushnumber(lua, sLevel);
		return 1;
	}

	bool TestLuaQuest::testSimple() {
		LuaQuest quest("lua1");
		LuaState &lua = quest.getLua();

		lua_getglobal(lua, "__quest");
		LuaQuest *questP = reinterpret_cast<LuaQuest *>(lua_touserdata(lua, -1));
		assert(questP == &quest);

		lua.pop(1);

		lua_register(lua, "getLevel", getLevel);
		assert(!lua.hasGlobalFunction("startQuest", true));
		quest.loadQuestString("function startQuest()\n"
			"	if getLevel() < 5 then\n"
			"		return false\n"
			"	else\n"
			"		return true\n"
			"	end\n"
			"end");
		assert(lua.hasGlobalFunction("startQuest", true));
		assert(!quest.startQuest());

		sLevel = 10;

		assert(quest.startQuest());
		
		return true;
	}

}
}