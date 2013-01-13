#pragma once

#include <string>
#include <map>
using namespace std;

#include <ui/event_interface.h>
using namespace am::ui;

namespace am {
namespace game {

	class Character;

	class Quest : public EventInterface {
	public:
		
		Quest(const char *questId);
		~Quest();

		virtual void setQuestId(const char *questId);
		virtual const char *getQuestId() const;

		virtual void setEnabled(bool enabled);
		virtual bool isEnabled() const;

		virtual bool startQuest();
		virtual bool finishQuest();

		virtual void setCompleted(bool completed);
		virtual bool isCompleted();

		virtual void setTitle(const char *title);
		virtual const char *getTitle() const;

		virtual void setDescription(const char *desc);
		virtual const char *getDescription() const;

		virtual void setActiveText(const char *text);
		virtual const char *getActiveText() const;

		virtual void setAcceptedReward(bool accepted);
		virtual bool hasAcceptedReward() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		string mQuestId;
		bool mAcceptedReward;
		bool mCompleted;
		bool mEnabled;

		string mTitle;
		string mDescription;
		string mActiveText;
	};

}
}
