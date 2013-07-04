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

	BodyPart::BodyPart(const char *name, BodyPartType::PartType type, Item *equipped) :
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
	
	void BodyPart::setType(BodyPartType::PartType type)
	{
		if (type < 0 || type >= BodyPartType::MAX_BODY_TYPE_LENGTH)
		{
			type = BodyPartType::UNKNOWN_PART;
		}
		mType = type;
	}
	BodyPartType::PartType BodyPart::getType() const
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

	bool BodyPart::setEquippedItem(Item *item, bool forceEquip)
	{
		// If we aren't forcing the equip or if the item is null
		// then we want to chance and it's a valid set.
		if (forceEquip || !item || mType == BodyPartType::UNKNOWN_PART || mType == BodyPartType::ANY)
		{
			mEquippedItem = item;
			return true;
		}
		// Otherwise we want to make sure that the types match.
		const BodyPartType::TypeList &typeList = item->getBodyPartTypeList();
		for (size_t i = 0; i < typeList.size(); i++)
		{
			if (typeList[i] == mType)
			{
				mEquippedItem = item;
				return true;
			}
		}
		return false;
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
		output->at("partType", BodyPartType::getBodyPartName(mType));
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
			setType(BodyPartType::getBodyPartType(str->string()));
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

}
}
