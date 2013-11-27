#pragma once

#include <base/handle.h>

#include "item.h"

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;

	class InventorySpot {
	public:
		InventorySpot();
		InventorySpot(Item *item, unsigned short x, unsigned short y);
		~InventorySpot();

		Item *getItem() const;
		unsigned short getX() const;
		unsigned short getY() const;

		friend class Inventory;

		data::IData *serialise();
		void deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		base::Handle<Item> mItem;
		unsigned short mX, mY;

	};

}
}
