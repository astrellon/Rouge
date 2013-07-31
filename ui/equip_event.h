#pragma once

#include <string>
using namespace std;

#include "event.h"

namespace am {
namespace game {
	class BodyPart;
	class Character;
	class Item;
}
using namespace am::game;

namespace ui {

	class EquipEvent : public Event {
	public:
		EquipEvent(const char *type, Character *character, BodyPart *part, Item *item);
		~EquipEvent();

		Character *getCharacter() const;
		Item *getItem() const;
		BodyPart *getBodyPart() const;

	protected:

		BodyPart *mPart;
		Character *mCharacter;
		Item *mItem;
	};

}
}
