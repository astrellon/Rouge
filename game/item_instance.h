#pragma once

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

#include <string>
using namespace std;

#include "item.h"

namespace am {
namespace game {

	class ItemInstance : public IManaged {
	public:
		ItemInstance();
		ItemInstance(Item *item);
		~ItemInstance();

		virtual void setItem(Item *item);
		virtual Item *getItem();

		virtual void setInventoryPosition(short x, short y);
		virtual short getInventoryPositionX() const;
		virtual short getInventoryPositionY() const;

		virtual void update(float dt);

	protected:
		
		Handle<Item> mItem;
		
		short mInventoryPosX;
		short mInventoryPosY;
	};

}
}
