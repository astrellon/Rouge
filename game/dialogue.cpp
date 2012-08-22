#include "dialogue.h"

namespace am {
namespace game {

	Dialogue::DialogueMap Dialogue::sDialogueMap;

	Dialogue::Dialogue(const char *id, const char *text) :
		mId(id),
		mText(text)
	{
	}
	Dialogue::~Dialogue()
	{
	}

	const char *Dialogue::getId() const
	{
		return mId.c_str();
	}

	void Dialogue::setText(const char *text)
	{
		mText = text;
	}
	const char *Dialogue::getText() const
	{
		return mText.c_str();
	}

	Dialogue::DialogueChoices &Dialogue::getChoices()
	{
		return mChoices;
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

}
}
