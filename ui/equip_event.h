#pragma once

#include <string>

#include "event.h"

namespace am {
namespace game {
	class BodyPart;
	class Character;
	class Item;
}

namespace ui {

	class EquipEvent : public Event 
	{
	public:
		EquipEvent(const char *type, game::Character *character, game::BodyPart *part, game::Item *item);
		~EquipEvent();

		game::Character *getCharacter() const;
		game::Item *getItem() const;
		game::BodyPart *getBodyPart() const;

	protected:

		game::BodyPart *mPart;
		game::Character *mCharacter;
		game::Item *mItem;
	};

}
}
