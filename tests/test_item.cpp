#include "test_item.h"

#include <game/item.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestItem::testSimple() {
		Handle<Item> item1(new Item());
		equalsStr("Item", item1->getFullItemName());
		equalsStr("Item", item1->getItemName());
		equalsStr("", item1->getPrefix());
		equalsStr("", item1->getPostfix());

		item1->setName("Sword");
		equalsStr("Sword", item1->getFullItemName());
		equalsStr("Sword", item1->getItemName());
		equalsStr("", item1->getPrefix());
		equalsStr("", item1->getPostfix());

		item1->setPrefix("Iron");
		equalsStr("Iron Sword", item1->getFullItemName());
		equalsStr("Sword", item1->getItemName());
		equalsStr("Iron", item1->getPrefix());
		equalsStr("", item1->getPostfix());

		item1->setPostfix("of Magic");
		equalsStr("Iron Sword of Magic", item1->getFullItemName());
		equalsStr("Sword", item1->getItemName());
		equalsStr("Iron", item1->getPrefix());
		equalsStr("of Magic", item1->getPostfix());

		item1->setPostfix("and Shield");
		equalsStr("Iron Sword and Shield", item1->getFullItemName());
		equalsStr("Sword", item1->getItemName());
		equalsStr("Iron", item1->getPrefix());
		equalsStr("and Shield", item1->getPostfix());

		item1->setPrefix(NULL);
		equalsStr("Sword and Shield", item1->getFullItemName());
		equalsStr("Sword", item1->getItemName());
		equalsStr("", item1->getPrefix());
		equalsStr("and Shield", item1->getPostfix());

		item1->setPostfix(NULL);
		equalsStr("Sword", item1->getFullItemName());
		equalsStr("Sword", item1->getItemName());
		equalsStr("", item1->getPrefix());
		equalsStr("", item1->getPostfix());

		item1->setItemFullname("Shield", "Wooden", "of broken");
		equalsStr("Wooden Shield of broken", item1->getFullItemName());
		equalsStr("Shield", item1->getItemName());
		equalsStr("Wooden", item1->getPrefix());
		equalsStr("of broken", item1->getPostfix());

		item1->setItemFullname("Wand", "Plain");
		equalsStr("Plain Wand", item1->getFullItemName());
		equalsStr("Wand", item1->getItemName());
		equalsStr("Plain", item1->getPrefix());
		equalsStr("", item1->getPostfix());
		
		item1->setItemFullname("Wand", NULL, "of Gamelon");
		equalsStr("Wand of Gamelon", item1->getFullItemName());
		equalsStr("Wand", item1->getItemName());
		equalsStr("", item1->getPrefix());
		equalsStr("of Gamelon", item1->getPostfix());


		return true;
	}

}
}
