#include "test_lua_quest.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/engine.h>
#include <game/game.h>
#include <game/quest.h>
using namespace am::game;

#include <util/utils.h>

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

		game::Engine *prevEng = game::Engine::getEngine();
		game::Engine *eng = new game::Engine();
		game::Engine::setEngine(eng);

		base::Handle<game::Game> game(new game::Game(eng));
		eng->setCurrentGame(game);

		base::Handle<game::Character> main(new game::Character());
		main->setGameId("testMainChar");
		game->setMainCharacter(main);

		int loadResult = lua.loadString(
			"local quest = am.quest.new(\"testQuest\")\n"
			"am.quest.add_quest(quest)\n"
			"local game = am.engine.game()\n"
			"local main = game:main()\n"
			"local questStarted = false\n"

			"function startQuest(event)\n"
			"	main:on(\"talk\", finishQuest)\n"
			"	main:off(\"talk\", startQuest)\n"
			"end\n"

			"function finishQuest(event)\n"
			"	main:off(\"talk\", finishQuest)\n"
			"	main:add_experience(1000)\n"
			"	main:coin_purse():add_coin(50)\n"
			"	quest:complete(true)\n"
			"end\n"
			"main:on(\"talk\", startQuest)\n"
			);

		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		lua_Debug ar;
		lua_getstack(lua, 1, &ar);

		game::Quest *quest = game::Engine::getGame()->getQuest("testQuest");
		assert(!quest->isCompleted());
		
		am_equals(0, main->getExperience());
		am_equals(0, main->getLevel());
		am_equals(0, main->getCoinPurse()->getCoin());

		base::Handle<ui::Event> startEvent(new ui::Event("talk"));
		main->fireEvent<ui::Event>(startEvent);
		main->fireEvent<ui::Event>(startEvent);

		am_equals(1000, main->getExperience());
		am_equals(1, main->getLevel());
		am_equals(50, main->getCoinPurse()->getCoin());
		am_equals(true, quest->isCompleted());

		game::Engine::setEngine(prevEng);

		delete eng;

		return true;
	}

}
}
