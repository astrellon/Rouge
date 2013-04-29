#include "inventory_spot.h"

#include <util/data_table.h>
#include <util/data_number.h>

#include <sstream>
#include <log/logger.h>

#include "loading_state.h"

namespace am {
namespace game {

	const int InventorySpot::LUA_ID = 0x0A;
	const char *InventorySpot::LUA_TABLENAME = "am_game_InventorySpot";

	InventorySpot::InventorySpot() :
		mX(0),
		mY(0)
	{

	}
	InventorySpot::InventorySpot(Item *item, unsigned short x, unsigned short y) :
		mItem(item),
		mX(x),
		mY(y)
	{

	}
	InventorySpot::~InventorySpot()
	{

	}

	Item *InventorySpot::getItem() const
	{
		return mItem;
	}
	unsigned short InventorySpot::getX() const
	{
		return mX;
	}
	unsigned short InventorySpot::getY() const
	{
		return mY;
	}

	data::IData *InventorySpot::serialise()
	{
		data::Table *output = new data::Table();
		output->push("x", mX);
		output->push("y", mY);
		if (mItem)
		{
			output->push("item", mItem->serialise());
		}
		return output;
	}
	void InventorySpot::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{
			stringstream ss;
			ss << "Unable to load inventory spot from a '" << data->typeName();
			ss << "', must be a Map.";
			am_log("LOADERR", ss);
			return;
		}

		Handle<data::Number> num(dataMap->at<data::Number>("x"));
		if (num)
		{
			mX = num->value<unsigned short>();
		}
		num = dataMap->at<data::Number>("y");
		if (num)
		{
			mY = num->value<unsigned short>();
		}

		Handle<data::IData> tempData(dataMap->at("item"));
		if (tempData)
		{
			mItem = new Item();
			mItem->deserialise(state, tempData);
		}
	}

}
}
