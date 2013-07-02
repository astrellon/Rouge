#include "body_part.h"

#include <util/data_table.h>
using namespace am::util;

#include "loading_state.h"

namespace am {
namespace game {

	const int BodyPart::LUA_ID = 0x03;
	const char *BodyPart::LUA_TABLENAME = "am_game_BodyPart";

	const char *BodyPart::sNiceBodyPartTypeNames[] = {
		"Hand", "Arm", "Leg", "Head", "Torso", "Neck", "Shoulders", "Legs", "Feet", "MAX_BODY_TYPE_LENGTH"
	};
	const char *BodyPart::sBodyPartTypeNames[] = {
		"hand", "arm", "leg", "head", "torso", "neck", "shoulders", "legs", "feet", "MAX_BODY_TYPE_LENGTH"
	};

	BodyPart::BodyPart(const char *name, BodyPartType type, Item *equipped) :
		mName(name),
		mType(type),
		mEquippedItem(equipped)
	{

	}
	BodyPart::BodyPart(const BodyPart &copy) :
		mName(copy.mName),
		mType(copy.mType)
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
	BodyPart::BodyPartType BodyPart::getType() const
	{
		return mType;
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
		data::Table *output = new data::Table();
		if (mEquippedItem)
		{
			output->at("equippedItem", mEquippedItem->serialise());
		}
		return output;
	}

	bool BodyPart::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Table> dataMap(data::Table::checkDataType(data, "body part"));
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

	const char *BodyPart::getNiceBodyPartName(BodyPartType type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return NULL;
		}
		return sNiceBodyPartTypeNames[type];
	}
	const char *BodyPart::getBodyPartName(BodyPartType type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return NULL;
		}
		return sBodyPartTypeNames[type];
	}
	const char *BodyPart::getBodyPartName(int type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return NULL;
		}
		return sBodyPartTypeNames[type];
	}
	
	BodyPart::BodyPartType BodyPart::getBodyPartType(int typeValue)
	{
		if (typeValue < 0 || typeValue > MAX_BODY_TYPE_LENGTH)
		{
			return MAX_BODY_TYPE_LENGTH;
		}
		return static_cast<BodyPartType>(typeValue);
	}
	BodyPart::BodyPartType BodyPart::getBodyPartTypeFromNice(const char *typeName)
	{
		if (typeName == NULL || typeName[0] == '\0')
		{
			return MAX_BODY_TYPE_LENGTH;
		}
		for (int i = 0; i < MAX_BODY_TYPE_LENGTH; i++)
		{
			if (strcmp(typeName, sNiceBodyPartTypeNames[i]) == 0)
			{
				return static_cast<BodyPartType>(i);
			}
		}
		return MAX_BODY_TYPE_LENGTH;
	}
	BodyPart::BodyPartType BodyPart::getBodyPartType(const char *typeName)
	{
		if (typeName == NULL || typeName[0] == '\0')
		{
			return MAX_BODY_TYPE_LENGTH;
		}
		for (int i = 0; i < MAX_BODY_TYPE_LENGTH; i++)
		{
			if (strcmp(typeName, sBodyPartTypeNames[i]) == 0)
			{
				return static_cast<BodyPartType>(i);
			}
		}
		return MAX_BODY_TYPE_LENGTH;
	}

}
}
