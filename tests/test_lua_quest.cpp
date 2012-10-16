#include "test_lua_quest.h"

#include <tests/asserts.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/lua_quest.h>
#include <game/engine.h>
#include <game/game.h>
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

		Engine *prevEng = Engine::getEngine();
		Engine *eng = new Engine();
		Engine::setEngine(eng);

		Handle<Game> game(new Game(eng));
		eng->setCurrentGame(game);

		Handle<Character> main(new Character());
		main->setGameId("testMainChar");
		game->setMainCharacter(main);

		int loadResult = lua.loadString("Character = import(\"Character\")\n"
			"Game = import(\"Game\")\n"
			"Engine = import(\"Engine\")\n"
			"local main = nil\n"
			"local game = Engine.get_current_game()\n"
			"main = game:get_main_character()\n"
			"function init()\n"
			"	main:add_event_listener(\"start_event\", startQuest)\n"
			"end\n"
			"function startQuest()\n"
			"	am_log(\"QUEST STARTED\")\n"
			"end\n"
			);

		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		assert(lua.hasGlobalFunction("init"));
		lua.call(0, 0);

		Handle<Event> startEvent(new Event("start_event"));
		main->fireEvent<Event>(startEvent);

		Engine::setEngine(prevEng);

		delete eng;

		return true;
	}

}
}