#include "lua_quest.h"

#include <sstream>
using namespace std;

namespace am {
namespace game {

	LuaQuest::LuaQuest(const char *questId) :
		Quest(questId)
	{
	}

	LuaQuest::~LuaQuest()
	{
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
	bool LuaQuest::checkComplete()
	{
		if (mLua.hasGlobalFunction("checkComplete"))
		{
			lua_call(mLua, 0, 1);
			bool result = lua_toboolean(mLua, -1) > 0;
			lua_pop(mLua, 1);
			return result;
		}
		return Quest::checkComplete();
	}

	void LuaQuest::setQuestProgress(int progress)
	{
		if (mLua.hasGlobalFunction("setQuestProgress"))
		{
			lua_call(mLua, 1, 1);
			int result = lua_tointeger(mLua, -1);
			lua_pop(mLua, 1);
			Quest::setQuestProgress(result);
		}
		else
		{
			Quest::setQuestProgress(progress);
		}
	}

	void LuaQuest::setTotalQuestProgress(int total)
	{
		if (mLua.hasGlobalFunction("setTotalQuestProgress"))
		{
			lua_call(mLua, 1, 1);
			int result = lua_tointeger(mLua, -1);
			lua_pop(mLua, 1);
			Quest::setTotalQuestProgress(result);
		}
		else
		{
			Quest::setTotalQuestProgress(total);
		}
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

}
}
