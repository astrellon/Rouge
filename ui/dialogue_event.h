#pragma once

#include <string>

#include "event.h"

namespace am {
namespace game {
	class Dialogue;
	class GameObject;
}

namespace ui {

	class DialogueEvent : public Event
	{
	public:
		DialogueEvent(game::Dialogue *dialogue, game::GameObject *talker, game::GameObject *talkedTo, bool addIdToEvent);
		~DialogueEvent();

		game::Dialogue *getDialogue() const;
		game::GameObject *getTalker() const;
		game::GameObject *getTalkedTo() const;

	protected:

		game::Dialogue *mDialogue;
        game::GameObject *mTalker;
        game::GameObject *mTalkedTo;

	};

}
}
