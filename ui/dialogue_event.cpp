#include "dialogue_event.h"

#include <game/game_object.h>

namespace am {
namespace ui {

	DialogueEvent::DialogueEvent(game::Dialogue *dialogue, bool addIdToEvent) :
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

	game::Dialogue *DialogueEvent::getDialogue() const
	{
		return mDialogue;
	}
	game::GameObject *DialogueEvent::getTalker() const
	{
		return dynamic_cast<game::GameObject *>(mEventTarget.get());
	}
	game::GameObject *DialogueEvent::getTalkedTo() const
	{
		game::GameObject *talker = getTalker();
		if (talker && talker->getDialogueComp())
		{
			return talker->getDialogueComp()->getTalkingTo();
		}
		return nullptr;
	}

}
}
