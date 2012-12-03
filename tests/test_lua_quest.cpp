#include "test_lua_quest.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/engine.h>
#include <game/game.h>
#include <game/quest.h>
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
		LuaState lua;

		Engine *prevEng = Engine::getEngine();
		Engine *eng = new Engine();
		Engine::setEngine(eng);

		Handle<Game> game(new Game(eng));
		eng->setCurrentGame(game);

		Handle<Character> main(new Character());
		main->setGameId("testMainChar");
		game->setMainCharacter(main);

		int loadResult = lua.loadString("Quest = import(\"Quest\")\n"
			"Character = import(\"Character\")\n"
			"Engine = import(\"Engine\")\n"
			"Game = import(\"Game\")\n"
			"CoinPurse = import(\"CoinPurse\")\n"

			"local quest = Quest.new(\"testQuest\")\n"
			"Quest.add_quest(quest)\n"
			"local game = Engine.get_current_game()\n"
			"local main = game:get_main_character()\n"

			"function startQuest(event)\n"
			"	main:remove_event_listener(\"talk\", startQuest)\n"
			"	main:add_event_listener(\"talk\", finishQuest)\n"
			"end\n"

			"function finishQuest(event)\n"
			"	main:remove_event_listener(\"talk\", finishQuest)\n"
			"	main:add_experience(1000)\n"
			"	main:get_coin_purse():add_coin(50)\n"
			"	quest:set_complete(true)\n"
			"end\n"
			"main:add_event_listener(\"talk\", startQuest)\n"
			);

		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Quest *quest = Engine::getGame()->getQuest("testQuest");
		assert(!quest->isCompleted());
		
		equals(0, main->getExperience());
		equals(0, main->getLevel());
		equals(0u, main->getCoinPurse()->getCoin());

		Handle<Event> startEvent(new Event("talk"));
		main->fireEvent<Event>(startEvent);
		main->fireEvent<Event>(startEvent);

		equals(1000, main->getExperience());
		equals(1, main->getLevel());
		equals(50u, main->getCoinPurse()->getCoin());

		Engine::setEngine(prevEng);

		delete eng;

		return true;
	}

}
}