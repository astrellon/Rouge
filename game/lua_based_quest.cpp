#include "lua_based_quest.h"

#include <sstream>
using namespace std;

#include "engine.h"
#include "game.h"
#include "character.h"

#include <lua/wrappers/lua_character.h>

namespace am {
namespace game {

	LuaQuest::LuaQuest(const char *questId) :
		Quest(questId)
	{
		lua_pushlightuserdata(mLua, this);
		lua_setglobal(mLua, "__quest");

		lua_pushcfunction(mLua, getMainCharacter);
		lua_setglobal(mLua, "getMainChar");
	}

	LuaQuest::~LuaQuest()
	{
		mLua.close();
	}

	bool LuaQuest::loadQuest(const char *questName)
	{
		if (questName == NULL || questName[0] == '\0')
		{
			questName = mQuestId.c_str();
		}
		stringstream ss;
		ss << "data/quests/" << questName << ".ssff";
		return mLua.loadFile(ss.str().c_str());
	}
	bool LuaQuest::loadQuestString(const char *questString)
	{
		if (questString == NULL || questString[0] == '\0')
		{
			return false;
		}
		return mLua.loadString(questString);
	}

	bool LuaQuest::startQuest()
	{
		if (mLua.hasGlobalFunction("startQuest"))
		{
			lua_call(mLua, 0, 1);
			bool result = lua_toboolean(mLua, -1) > 0;
			lua_pop(mLua, 1);
			return result;
		}
		return Quest::startQuest();
	}
	bool LuaQuest::finishQuest()
	{
		return Quest::finishQuest();
	}

	void LuaQuest::setCompleted(bool completed)
	{
		Quest::setCompleted(completed);
	}
	bool LuaQuest::isCompleted()
	{
		if (mLua.hasGlobalFunction("checkComplete"))
		{
			lua_call(mLua, 0, 1);
			bool result = lua_toboolean(mLua, -1) > 0;
			lua_pop(mLua, 1);
			return result;
		}
		return Quest::isCompleted();
	}

	const char *LuaQuest::getTitle()
	{
		return Quest::getTitle();
	}
	const char *LuaQuest::getDescription()
	{
		return Quest::getDescription();
	}
	const char *LuaQuest::getActiveText()
	{
		return Quest::getActiveText();
	}

	void LuaQuest::setAcceptedReward(bool accepted)
	{
		if (mLua.hasGlobalFunction("setAcceptedReward"))
		{
			lua_call(mLua, 1, 1);
			bool result = lua_toboolean(mLua, -1) > 0;
			lua_pop(mLua, 1);
			Quest::setAcceptedReward(result);
		}
		else
		{
			Quest::setAcceptedReward(accepted);
		}
	}

	LuaState &LuaQuest::getLua()
	{
		return mLua;
	}

	int LuaQuest::getMainCharacter(lua_State *lua)
	{
		Engine *eng = Engine::getEngine();
		if (eng)
		{
			Game *game = eng->getCurrentGame();
			if (game)
			{
				Character *main = game->getMainCharacter();
				if (main)
				{
					am::lua::game::Character_wrap(lua, main);
					return 1;
				}
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
