#include "body_part.h"

#include <util/data_table.h>
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

}
}
