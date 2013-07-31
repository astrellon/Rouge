#include "equip_event.h"

#include <game/character.h>
#include <game/item.h>
#include <game/body_part.h>

namespace am {
namespace ui {

	EquipEvent::EquipEvent(const char *type, Character *character, BodyPart *part, Item *item) :
		Event(type),
		mCharacter(character),
		mPart(part),
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
		if (part)
		{
			part->retain();
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
		if (mPart)
		{
			mPart->release();
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
	BodyPart *EquipEvent::getBodyPart() const
	{
		return mPart;
	}

}
}
