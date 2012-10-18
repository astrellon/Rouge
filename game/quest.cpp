#include "quest.h"

namespace am {
namespace game {

	Quest::QuestMap Quest::sQuestMap;

	Quest::Quest(const char *questId) :
		mQuestId(questId)
	{

	}
	Quest::~Quest()
	{

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

	const char *Quest::getTitle()
	{
		return "No Title";
	}
	const char *Quest::getDescription()
	{
		return "No Description";
	}
	const char *Quest::getActiveText()
	{
		return "No Active Text";
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

	bool Quest::addQuest(Quest *quest)
	{
		if (quest == NULL)
		{
			return false;
		}
		QuestMap::const_iterator iter = sQuestMap.find(quest->mQuestId);
		if (iter == sQuestMap.end())
		{
			sQuestMap[quest->mQuestId] = quest;
			return true;
		}
		return  false;
	}
	bool Quest::removeQuest(const char *questId)
	{
		if (questId == NULL || questId[0] == '\0')
		{
			return false;
		}
		QuestMap::const_iterator iter = sQuestMap.find(string(questId));
		if (iter != sQuestMap.end())
		{
			sQuestMap.erase(iter);
			return true;
		}
		return  false;
	}
	Quest *Quest::getQuest(const char *questId)
	{
		if (questId == NULL || questId[0] == '\0')
		{
			return NULL;
		}
		QuestMap::const_iterator iter = sQuestMap.find(string(questId));
		if (iter != sQuestMap.end())
		{
			return iter->second;
		}
		return NULL;
	}

}
}
