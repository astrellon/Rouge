#include "dialogue_event.h"

#include <game/character.h>
#include <game/game_object.h>

namespace am {
namespace ui {

	DialogueEvent::DialogueEvent(GameObject *talker, GameObject *talkedTo, Dialogue *dialogue) :
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

	GameObject *DialogueEvent::getTalker() const
	{
		return mTalker;
	}
	GameObject *DialogueEvent::getTalkedTo() const
	{
		return mTalkedTo;
	}
	Dialogue *DialogueEvent::getDialogue() const
	{
		return mDialogue;
	}
}
}
