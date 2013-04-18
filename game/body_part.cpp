#include "body_part.h"

#include <util/data_map.h>
using namespace am::util;

namespace am {
namespace game {

	const int BodyPart::LUA_ID = 0x03;
	const char *BodyPart::LUA_TABLENAME = "am_game_BodyPart";

	BodyPart::BodyPart(const char *name, Item *equipped) :
		mName(name),
		mEquippedItem(equipped)
	{

	}
	BodyPart::BodyPart(const BodyPart &copy) :
		mName(copy.mName)
	{
		if (copy.mEquippedItem)
		{
			mEquippedItem = new Item(*copy.mEquippedItem);
		}
	}
	BodyPart::~BodyPart()
	{

	}

	const char *BodyPart::getName() const
	{
		return mName.c_str();
	}

	void BodyPart::setEquippedItem(Item *item)
	{
		mEquippedItem = item;
	}
	Item *BodyPart::getEqippedItem() const
	{
		return mEquippedItem;
	}

	data::IData *BodyPart::serialise()
	{
		data::Map *output = new data::Map();
		output->push("name", mName);
		if (mEquippedItem)
		{
			output->push("equippedItem", mEquippedItem->serialise());
		}
		return output;
	}

	/*void BodyPart::addBodyPart(BodyPart *part)
	{
		if (part == NULL)
		{
			return;
		}
		string name = part->getName();
		sBodyParts[name] = part;
	}
	BodyPart *BodyPart::getBodyPart(const char *name)
	{
		if (name == NULL || name[0] == '\0')
		{
			return NULL;
		}
		string nameStr = name;
		BodyPartMap::iterator iter = sBodyParts.find(nameStr);
		if (iter == sBodyParts.end())
		{
			return NULL;
		}
		return iter->second;
	}*/

}
}
