#include "body_part.h"

#include <util/data_map.h>
#include <util/data_array.h>
using namespace am::util;

#include "loading_state.h"

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
		if (mEquippedItem)
		{
			output->push("equippedItem", mEquippedItem->serialise());
		}
		return output;
	}

	bool BodyPart::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Map> dataMap(data::Map::checkDataType(data, "body part"));
		if (!dataMap)
		{
			return false;
		}

		Handle<data::IData> tempData(dataMap->at("equippedItem"));
		if (tempData)
		{
			Handle<Item> item(new Item());
			if (item->deserialise(state, tempData) == 1)
			{
				setEquippedItem(item);
			}
		}
		return true;
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
