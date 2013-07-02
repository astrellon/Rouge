#include "body_part.h"

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
using namespace am::util;

#include "loading_state.h"

namespace am {
namespace game {

	const int BodyPart::LUA_ID = 0x03;
	const char *BodyPart::LUA_TABLENAME = "am_game_BodyPart";

	const char *BodyPart::sNiceBodyPartTypeNames[] = {
		"Unknown Part", "Hand", "Arm", "Leg", "Head", "Torso", "Neck", "Shoulders", "Legs", "Feet", "MAX_BODY_TYPE_LENGTH"
	};
	const char *BodyPart::sBodyPartTypeNames[] = {
		"unknown_part", "hand", "arm", "leg", "head", "torso", "neck", "shoulders", "legs", "feet", "MAX_BODY_TYPE_LENGTH"
	};

	BodyPart::BodyPart(const char *name, BodyPartType type, Item *equipped) :
		mName(name),
		mType(type),
		mEquippedItem(equipped),
		mMainWeapon(false),
		mOffWeapon(false)
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
	
	void BodyPart::setType(BodyPart::BodyPartType type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			type = UNKNOWN_PART;
		}
		mType = type;
	}
	BodyPart::BodyPartType BodyPart::getType() const
	{
		return mType;
	}

	void BodyPart::setMainWeapon(bool mainWeapon)
	{
		mMainWeapon = mainWeapon;
	}
	bool BodyPart::isMainWeapon() const
	{
		return mMainWeapon;
	}

	void BodyPart::setOffWeapon(bool offWeapon)
	{
		mOffWeapon = offWeapon;
	}
	bool BodyPart::isOffWeapon() const
	{
		return mOffWeapon;
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
		output->at("partType", getBodyPartName(mType));
		output->at("mainWeapon", mMainWeapon);
		output->at("offWeapon", mOffWeapon);
		return output;
	}

	bool BodyPart::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Table> dataMap(data::Table::checkDataType(data, "body part"));
		if (!dataMap)
		{
			return false;
		}

		Handle<data::String> str(dataMap->at<data::String>("partType"));
		if (str)
		{
			setType(getBodyPartType(str->string()));
		}

		Handle<data::Boolean> boo(dataMap->at<data::Boolean>("mainWeapon"));
		if (boo)
		{
			setMainWeapon(boo->boolean());
		}
		boo = dataMap->at<data::Boolean>("offWeapon");
		if (boo)
		{
			setOffWeapon(boo->boolean());
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
