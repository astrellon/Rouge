#include "dialogue_event.h"

#include <game/game_object.h>
using namespace am::game;

namespace am {
namespace ui {

	DialogueEvent::DialogueEvent(Dialogue *dialogue) :
		Event("dialogue"),
		mDialogue(dialogue)
	{
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
		if (talker)
		{
			return talker->getTalkingTo();
		}
		return NULL;
	}

}
}
