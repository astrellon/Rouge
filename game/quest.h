#pragma once

#include <string>
#include <map>
using namespace std;

#include <ui/event_manager.h>
using namespace am::ui;

namespace am {
namespace game {

	class Character;

	class Quest : public EventManager {
	public:
		
		Quest(const char *questId);
		~Quest();

		virtual void setQuestId(const char *questId);
		virtual const char *getQuestId() const;

		virtual bool startQuest();
		virtual bool finishQuest();

		virtual void setCompleted(bool completed);
		virtual bool isCompleted();

		virtual const char *getTitle();
		virtual const char *getDescription();
		virtual const char *getActiveText();

		virtual void setAcceptedReward(bool accepted);
		virtual bool hasAcceptedReward() const;

		static bool addQuest(Quest *quest);
		static bool removeQuest(const char *questId);
		static Quest *getQuest(const char *questId);

	protected:

		string mQuestId;
		bool mAcceptedReward;
		bool mCompleted;

		typedef map<string, Quest *> QuestMap;
		static QuestMap sQuestMap;
	};

}
}
