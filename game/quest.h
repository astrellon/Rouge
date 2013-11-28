#pragma once

#include <base/imanaged.h>

#include <string>
#include <map>

#include <ui/event_interface.h>

#include <game/iattribute_data.h>

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class Character;
	class LoadingState;

	class Quest : public ui::EventInterface, public IAttributeData, public virtual base::IManaged 
	{
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

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		std::string mQuestId;
		bool mAcceptedReward;
		bool mCompleted;
		bool mEnabled;

		std::string mTitle;
		std::string mDescription;
		std::string mActiveText;
	};

}
}
