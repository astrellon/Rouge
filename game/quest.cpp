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
		if (mQuestProgress >= 0)
		{
			return false;
		}
		mQuestProgress = 0;
		return true;
	}
	bool Quest::checkComplete()
	{
		if (mQuestProgress < mTotalQuestProgress)
		{
			return false;
		}

		return true;
	}

	void Quest::setQuestId(const char *questId)
	{
		mQuestId = questId;
	}
	const char *Quest::getQuestId() const
	{
		return mQuestId.c_str();
	}

	void Quest::setQuestProgress(int progress)
	{
		mQuestProgress = progress;
	}
	int Quest::getQuestProgress() const
	{
		return mQuestProgress;
	}

	void Quest::setTotalQuestProgress(int total)
	{
		mTotalQuestProgress = total;
	}
	int Quest::getTotalQuestProgress() const
	{
		return mTotalQuestProgress;
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
