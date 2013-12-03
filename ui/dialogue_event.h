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
		DialogueEvent(game::Dialogue *dialogue, bool addIdToEvent);
		~DialogueEvent();

		game::Dialogue *getDialogue() const;
		game::GameObject *getTalker() const;
		game::GameObject *getTalkedTo() const;

	protected:

		game::Dialogue *mDialogue;

	};

}
}
