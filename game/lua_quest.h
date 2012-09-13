#pragma once

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/quest.h>

namespace am {
namespace game {

	class LuaQuest : public Quest {
	public:
		
		LuaQuest(const char *questId);
		~LuaQuest();

		bool loadQuest(const char *questName = NULL);
		bool loadQuestString(const char *questString);

		virtual bool startQuest();
		virtual bool checkComplete();

		virtual void setQuestProgress(int progress);
		virtual void setTotalQuestProgress(int total);
		virtual void setAcceptedReward(bool accepted);

	protected:

		LuaState mLua;
		
	};

}
}
