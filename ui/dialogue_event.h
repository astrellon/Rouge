#pragma once

#include <string>
using namespace std;

#include "event.h"

namespace am {
namespace game {
	class GameObject;
	class Dialogue;
}
using namespace am::game;

namespace ui {

	class DialogueEvent : public Event {
	public:
		DialogueEvent(GameObject *talker, GameObject *talkedTo, Dialogue *dialogue);
		~DialogueEvent();

		GameObject *getTalker() const;
		GameObject *getTalkedTo() const;
		Dialogue *getDialogue() const;

	protected:

		GameObject *mTalker;
		GameObject *mTalkedTo;
		Dialogue *mDialogue;

	};

}
}
