#pragma once

#include <string>
using namespace std;

#include "event.h"

namespace am {
namespace game {
	class Character;
	class Dialogue;
}
using namespace am::game;

namespace ui {

	class DialogueEvent : public Event {
	public:
		DialogueEvent(Character *talker, Character *talkedTo, Dialogue *dialogue);
		~DialogueEvent();

		Character *getTalker() const;
		Character *getTalkedTo() const;
		Dialogue *getDialogue() const;

	protected:

		Character *mTalker;
		Character *mTalkedTo;
		Dialogue *mDialogue;

	};

}
}
