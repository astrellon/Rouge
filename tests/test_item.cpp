#include "test_item.h"

#include <game/item.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestItem::testSimple() {
		am::base::Handle<Item> item1(new Item());
		am_equalsStr("Item", item1->getFullItemName());
		am_equalsStr("Item", item1->getItemName());
		am_equalsStr("", item1->getPrefix());
		am_equalsStr("", item1->getPostfix());

		item1->setName("Sword");
		am_equalsStr("Sword", item1->getFullItemName());
		am_equalsStr("Sword", item1->getItemName());
		am_equalsStr("", item1->getPrefix());
		am_equalsStr("", item1->getPostfix());

		item1->setPrefix("Iron");
		am_equalsStr("Iron Sword", item1->getFullItemName());
		am_equalsStr("Sword", item1->getItemName());
		am_equalsStr("Iron", item1->getPrefix());
		am_equalsStr("", item1->getPostfix());

		item1->setPostfix("of Magic");
		am_equalsStr("Iron Sword of Magic", item1->getFullItemName());
		am_equalsStr("Sword", item1->getItemName());
		am_equalsStr("Iron", item1->getPrefix());
		am_equalsStr("of Magic", item1->getPostfix());

		item1->setPostfix("and Shield");
		am_equalsStr("Iron Sword and Shield", item1->getFullItemName());
		am_equalsStr("Sword", item1->getItemName());
		am_equalsStr("Iron", item1->getPrefix());
		am_equalsStr("and Shield", item1->getPostfix());

		item1->setPrefix(nullptr);
		am_equalsStr("Sword and Shield", item1->getFullItemName());
		am_equalsStr("Sword", item1->getItemName());
		am_equalsStr("", item1->getPrefix());
		am_equalsStr("and Shield", item1->getPostfix());

		item1->setPostfix(nullptr);
		am_equalsStr("Sword", item1->getFullItemName());
		am_equalsStr("Sword", item1->getItemName());
		am_equalsStr("", item1->getPrefix());
		am_equalsStr("", item1->getPostfix());

		item1->setItemFullname("Shield", "Wooden", "of broken");
		am_equalsStr("Wooden Shield of broken", item1->getFullItemName());
		am_equalsStr("Shield", item1->getItemName());
		am_equalsStr("Wooden", item1->getPrefix());
		am_equalsStr("of broken", item1->getPostfix());

		item1->setItemFullname("Wand", "Plain");
		am_equalsStr("Plain Wand", item1->getFullItemName());
		am_equalsStr("Wand", item1->getItemName());
		am_equalsStr("Plain", item1->getPrefix());
		am_equalsStr("", item1->getPostfix());
		
		item1->setItemFullname("Wand", nullptr, "of Gamelon");
		am_equalsStr("Wand of Gamelon", item1->getFullItemName());
		am_equalsStr("Wand", item1->getItemName());
		am_equalsStr("", item1->getPrefix());
		am_equalsStr("of Gamelon", item1->getPostfix());


		return true;
	}

}
}
