#pragma once

#include <string>

#include "event.h"

namespace am {
namespace game {
	class Dialogue;
	class GameObject;
}
using namespace am::game;

namespace ui {

	class DialogueEvent : public Event {
	public:
		DialogueEvent(Dialogue *dialogue, bool addIdToEvent);
		~DialogueEvent();

		Dialogue *getDialogue() const;
		GameObject *getTalker() const;
		GameObject *getTalkedTo() const;

	protected:

		Dialogue *mDialogue;

	};

}
}
