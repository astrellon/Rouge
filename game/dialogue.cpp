#include "dialogue.h"

#include "character.h"

namespace am {
namespace game {

	Dialogue::DialogueMap Dialogue::sDialogueMap;

	Dialogue::Dialogue(const char *id, const char *text, const char *title, const char *subject, UnlockFlag unlock) :
		mId(id),
		mText(text),
		mUnlockFlag(unlock)
	{
		if (title != NULL)
		{
			mTitle = title;
		}
		if (subject != NULL)
		{
			mSubject = subject;
		}
	}
	Dialogue::~Dialogue()
	{
	}

	void Dialogue::setText(const char *text)
	{
		mText = text;
	}
	const char *Dialogue::getText() const
	{
		return mText.c_str();
	}

	void Dialogue::setTitle(const char *title)
	{
		mTitle = title;
	}
	const char *Dialogue::getTitle() const
	{
		if (mTitle.length() > 0)
		{
			return mTitle.c_str();
		}
		return mId.c_str();
	}

	void Dialogue::setId(const char *id)
	{
		mId = id;
	}
	const char *Dialogue::getId() const
	{
		return mId.c_str();
	}

	void Dialogue::setSubject(const char *subject)
	{
		mSubject = subject;
	}
	const char *Dialogue::getSubject() const
	{
		if (mSubject.length() > 0)
		{
			return mSubject.c_str();
		}
		return mId.c_str();
	}

	void Dialogue::setUnlockFlag(Dialogue::UnlockFlag flag)
	{
		mUnlockFlag = flag;
	}
	Dialogue::UnlockFlag Dialogue::getUnlockFlag() const
	{
		return mUnlockFlag;
	}

	bool Dialogue::addDialogue(Dialogue *dialogue)
	{
		if (dialogue)
		{
			string id = dialogue->getId();
			DialogueMap::const_iterator iter = sDialogueMap.find(id);
			if (iter == sDialogueMap.end())
			{
				sDialogueMap[id] = dialogue;
				return true;
			}
		}
		return false;
	}
	bool Dialogue::removeDialogue(const char *id)
	{
		if (id != NULL)
		{
			DialogueMap::const_iterator iter = sDialogueMap.find(string(id));
			if (iter == sDialogueMap.end())
			{
				sDialogueMap.erase(iter);
				return true;
			}
		}
		return false;
	}
	void Dialogue::removeAllDialogue()
	{
		sDialogueMap.clear();
	}
	Dialogue *Dialogue::getDialogue(const char *id)
	{
		if (id != NULL)
		{
			DialogueMap::iterator iter = sDialogueMap.find(string(id));
			if (iter != sDialogueMap.end())
			{
				return iter->second;
			}
		}
		return NULL;
	}

	void Dialogue::getAvailableDialogues(vector<Dialogue *> &result, const Character *talker, const Character *talkedTo)
	{
		if (talker == NULL || talkedTo == NULL)
		{
			return;
		}

		const Character::SubjectMap &unlocked = talker->getUnlockedSubjects();
		const Character::SubjectMap &available = talkedTo->getDialoguesAvailable();
		Character::SubjectMap::const_iterator iter;
		for (iter = available.begin(); iter != available.end(); ++iter)
		{
			Dialogue *dialogue = getDialogue(iter->first.c_str());
			if (dialogue == NULL)
			{
				continue;
			}
			if (dialogue->getUnlockFlag() == NONE)
			{
				result.push_back(dialogue);
			}
			else if (dialogue->getUnlockFlag() == LOCKED)
			{
				// If the subject of the dialogue is in the unlocked map and is true.
				Character::SubjectMap::const_iterator iter = unlocked.find(string(dialogue->getSubject()));
				if (iter != unlocked.end() && iter->second)
				{
					result.push_back(dialogue);
				}
			}
		}
	}

}
}
