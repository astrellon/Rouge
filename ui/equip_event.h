#pragma once

#include <string>
using namespace std;

#include "event.h"

namespace am {
namespace game {
	class Character;
	class Item;
}
using namespace am::game;

namespace ui {

	class EquipEvent : public Event {
	public:
		EquipEvent(const char *type, Character *character, const char *bodyPartName, Item *item);
		~EquipEvent();

		Character *getCharacter() const;
		Item *getItem() const;
		const char *getBodyPartName() const;

	protected:

		string mBodyPartName;
		Character *mCharacter;
		Item *mItem;
	};

}
}
