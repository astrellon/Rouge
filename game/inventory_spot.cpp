#include "inventory_spot.h"

#include <util/data_table.h>
#include <util/data_number.h>

#include <sstream>
#include <log/logger.h>

#include "loading_state.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int InventorySpot::LUA_ID = LUA_ID_INVENTORYSPOT;
	const char *InventorySpot::LUA_TABLENAME = LUA_TABLE_INVENTORYSPOT;

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
		output->at("x", mX);
		output->at("y", mY);
		if (mItem)
		{
			output->at("item", mItem->serialise());
		}
		return output;
	}
	void InventorySpot::deserialise(LoadingState *state, data::IData *data)
	{
		am::base::Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{
			std::stringstream ss;
			ss << "Unable to load inventory spot from a '" << data->typeName();
			ss << "', must be a Map.";
			am_log("LOADERR", ss);
			return;
		}

		am::base::Handle<data::Number> num(dataMap->at<data::Number>("x"));
		if (num)
		{
			mX = num->value<unsigned short>();
		}
		num = dataMap->at<data::Number>("y");
		if (num)
		{
			mY = num->value<unsigned short>();
		}

		am::base::Handle<data::IData> tempData(dataMap->at("item"));
		if (tempData)
		{
			mItem = new Item();
			mItem->deserialise(state, tempData);
		}
	}

}
}
