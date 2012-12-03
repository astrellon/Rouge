#include "quest.h"

namespace am {
namespace game {

	//Quest::QuestMap Quest::sQuestMap;

	Quest::Quest(const char *questId) :
		mQuestId(questId),
		mEnabled(true),
		mAcceptedReward(false),
		mCompleted(false),
		mTitle("No Title"),
		mDescription("No Description"),
		mActiveText("No Active Text")
	{

	}
	Quest::~Quest()
	{

	}

	void Quest::setEnabled(bool enabled)
	{
		mEnabled = enabled;
	}
	bool Quest::isEnabled() const
	{
		return mEnabled;
	}

	bool Quest::startQuest()
	{
		if (isCompleted())
		{
			return false;
		}
		return true;
	}
	bool Quest::finishQuest()
	{
		if (isCompleted())
		{
			return false;
		}
		setCompleted(true);
		return true;
	}

	void Quest::setCompleted(bool completed)
	{
		mCompleted = completed;
	}
	bool Quest::isCompleted()
	{
		return mCompleted;
	}

	void Quest::setTitle(const char *title)
	{
		mTitle = title;
	}
	const char *Quest::getTitle() const
	{
		return mTitle.c_str();;
	}

	void Quest::setDescription(const char *desc)
	{
		mDescription = desc;
	}
	const char *Quest::getDescription() const
	{
		return mDescription.c_str();
	}

	void Quest::setActiveText(const char *text)
	{
		mActiveText = text;
	}
	const char *Quest::getActiveText() const
	{
		return mActiveText.c_str();
	}

	void Quest::setQuestId(const char *questId)
	{
		mQuestId = questId;
	}
	const char *Quest::getQuestId() const
	{
		return mQuestId.c_str();
	}

	void Quest::setAcceptedReward(bool accepted)
	{
		mAcceptedReward = accepted;
	}
	bool Quest::hasAcceptedReward() const
	{
		return mAcceptedReward;
	}

}
}
