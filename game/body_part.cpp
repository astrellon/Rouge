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

	// Protected ctor
	BodyPart::BodyPart() :
		mType(BodyPartType::UNKNOWN_PART),
		mWeaponPart(false),
		//mOffWeapon(false),
		mIsHoldingOnto(false)
	{
	}
	BodyPart::BodyPart(const char *name, BodyPartType::PartType type, Item *equipped) :
		mName(name),
		mType(type),
		mEquippedItem(equipped),
		mWeaponPart(false),
		//mOffWeapon(false),
		mIsHoldingOnto(false)
	{
	}
	BodyPart::BodyPart(const BodyPart &copy) :
		mName(copy.mName),
		mType(copy.mType),
		mWeaponPart(copy.mWeaponPart),
		//mOffWeapon(copy.mOffWeapon),
		mIsHoldingOnto(false)
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

	void BodyPart::setWeaponPart(bool weaponPart)
	{
		mWeaponPart = weaponPart;
	}
	bool BodyPart::isWeaponPart() const
	{
		return mWeaponPart;
	}

	/*void BodyPart::setOffWeapon(bool offWeapon)
	{
		mOffWeapon = offWeapon;
	}
	bool BodyPart::isOffWeapon() const
	{
		return mOffWeapon;
	}*/

	void BodyPart::setCanHoldOnto(BodyPart *part)
	{
		mCanHoldOnto = part;
	}
	BodyPart *BodyPart::getCanHoldOnto() const
	{
		return mCanHoldOnto;
	}

	void BodyPart::setIsHoldingOnto(bool setHolding)
	{
		mIsHoldingOnto = setHolding;
	}
	bool BodyPart::isHoldingOnto() const
	{
		return mIsHoldingOnto;
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
		if (canEquipItem(item))
		{
			mEquippedItem = item;
		}
		return false;
	}
	bool BodyPart::canEquipItem(Item *item) const
	{
		const BodyPartType::TypeList &typeList = item->getBodyPartTypeList();
		for (size_t i = 0; i < typeList.size(); i++)
		{
			if (typeList[i] == mType)
			{
				return true;
			}
		}
		return false;
	}
	Item *BodyPart::getEquippedItem() const
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
		output->at("partName", mName);
		output->at("isWeaponPart", mWeaponPart);
		//output->at("offWeapon", mOffWeapon);
		if (mCanHoldOnto)
		{
			output->at("canHoldOnto", mCanHoldOnto->getName());
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

		Handle<data::String> str(dataMap->at<data::String>("partName"));
		if (str)
		{
			mName = str->string();
		}
		else if (mName.empty())
		{
			return false;
		}

		str = dataMap->at<data::String>("partType");
		if (str)
		{
			setType(BodyPartType::getBodyPartType(str->string()));
		}

		Handle<data::Boolean> boo(dataMap->at<data::Boolean>("isWeaponPart"));
		if (boo)
		{
			setWeaponPart(boo->boolean());
		}
		/*boo = dataMap->at<data::Boolean>("offWeapon");
		if (boo)
		{
			setOffWeapon(boo->boolean());
		}*/

		Handle<Item> item(new Item());
		Handle<data::IData> tempData(dataMap->at("equippedItem"));
		if (tempData)
		{
			if (item->deserialise(state, tempData) != 1)
			{
				//setEquippedItem(item);
				item = NULL;
			}
		}

		// Can hold onto it deserialised by the loading state.
		string canHoldOnto;
		str = dataMap->at<data::String>("canHoldOnto");
		if (str)
		{
			canHoldOnto = str->string();
		}

		state->addBodyPartInfo(state->getCurrentCharacter(), this, item, canHoldOnto.c_str());
		return true;
	}
	BodyPart *BodyPart::fromDeserialise(LoadingState *state, data::IData *data)
	{
		BodyPart *part = new BodyPart();
		if (!part->deserialise(state, data))
		{
			delete part;
			part = NULL;
		}
		return part;
	}

}
}
