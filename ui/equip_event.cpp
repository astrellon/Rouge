#include "equip_event.h"

#include <game/character.h>
#include <game/item.h>

namespace am {
namespace ui {

	EquipEvent::EquipEvent(const char *type, Character *character, const char *bodyPartName, Item *item) :
		Event(type),
		mCharacter(character),
		mBodyPartName(bodyPartName),
		mItem(item)
	{
		if (character)
		{
			character->retain();
		}
		if (item)
		{
			item->retain();
		}
	}
	EquipEvent::~EquipEvent()
	{
		if (mCharacter)
		{
			mCharacter->release();
		}
		if (mItem)
		{
			mItem->release();
		}
	}

	Character *EquipEvent::getCharacter() const
	{
		return mCharacter;
	}
	Item *EquipEvent::getItem() const
	{
		return mItem;
	}
	const char *EquipEvent::getBodyPartName() const
	{
		return mBodyPartName.c_str();
	}
}
}
