#include "dialogue.h"

namespace am {
namespace game {

	//Dialogue::DialogueSelectorList Dialogue::sDialogueList;
	Dialogue::DialogueMap Dialogue::sDialogueMap;
	Dialogue::SubjectMap Dialogue::sSubjectMap;

	Dialogue::Dialogue(const char *id, const char *text, const char *title, const char *subject) :
		mId(id),
		mText(text)
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

	/*const char *Dialogue::getId() const
	{
		return mId.c_str();
	}*/

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
	/*Dialogue::DialogueChoices &Dialogue::getChoices()
	{
		return mChoices;
	}*/

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

	bool Dialogue::setSubjectFlag(const char *subject, int flag)
	{
		if (subject == NULL || subject[0] == '\0')
		{
			return false;
		}
		string subStr = subject;
		SubjectMap::iterator iter = sSubjectMap.find(subStr);
		if (iter == sSubjectMap.end() || iter->second != flag)
		{
			sSubjectMap[subStr] = flag;
			return true;
		}
		return false;
	}
	bool Dialogue::getSubjectFlag(const char *subject, int &flag)
	{
		if (subject == NULL || subject[0] == '\0')
		{
			return false;
		}
		SubjectMap::const_iterator iter = sSubjectMap.find(string(subject));
		if (iter != sSubjectMap.end())
		{
			flag = iter->second;
			return true;
		}
		return false;
	}
}
}
