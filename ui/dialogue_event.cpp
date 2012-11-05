#include "dialogue_event.h"

#include <game/character.h>
#include <game/dialogue.h>

namespace am {
namespace ui {

	DialogueEvent::DialogueEvent(Character *talker, Character *talkedTo, Dialogue *dialogue) :
		Event("dialogue"),
		mTalker(talker),
		mTalkedTo(talkedTo),
		mDialogue(dialogue)
	{
		if (talker)
		{
			talker->retain();
		}
		if (talkedTo)
		{
			talkedTo->retain();
		}
	}
	DialogueEvent::~DialogueEvent()
	{
		if (mTalker)
		{
			mTalker->release();
		}
		if (mTalkedTo)
		{
			mTalkedTo->release();
		}
	}

	Character *DialogueEvent::getTalker() const
	{
		return mTalker;
	}
	Character *DialogueEvent::getTalkedTo() const
	{
		return mTalkedTo;
	}
	Dialogue *DialogueEvent::getDialogue() const
	{
		return mDialogue;
	}
}
}
