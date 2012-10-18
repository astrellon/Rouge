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
		virtual bool finishQuest();

		virtual void setCompleted(bool completed);
		virtual bool isCompleted();

		virtual const char *getTitle();
		virtual const char *getDescription();
		virtual const char *getActiveText();

		virtual void setAcceptedReward(bool accepted);
		
		virtual LuaState &getLua();

	protected:

		LuaState mLua;
		
	};

}
}
