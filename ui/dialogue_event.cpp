#include "dialogue_event.h"

#include <game/game_object.h>
using namespace am::game;

namespace am {
namespace ui {

	DialogueEvent::DialogueEvent(Dialogue *dialogue, bool addIdToEvent) :
		Event(),
		mDialogue(dialogue)
	{
		string ss = "dialogue";
		if (addIdToEvent && dialogue)
		{
			ss += ':';
			ss += dialogue->getId();
		}
		mType = ss;
	}
	DialogueEvent::~DialogueEvent()
	{
	}

	Dialogue *DialogueEvent::getDialogue() const
	{
		return mDialogue;
	}
	GameObject *DialogueEvent::getTalker() const
	{
		return dynamic_cast<GameObject *>(mEventTarget);
	}
	GameObject *DialogueEvent::getTalkedTo() const
	{
		GameObject *talker = getTalker();
		if (talker && talker->getDialogueComp())
		{
			return talker->getDialogueComp()->getTalkingTo();
		}
		return nullptr;
	}

}
}
