#include "test_store.h"

#include <game/store.h>
#include <game/character.h>
#include <game/inventory.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestStore::testSimple()
	{
		Handle<Store> store(new Store());
		Handle<Inventory> storeInv(new Inventory(10, 10));
		Handle<Item> testItem(new Item());
		testItem->setInventorySize(2, 2);
		testItem->setItemValue(50);
		storeInv->addItem(testItem);
		am_equals(true, storeInv->hasItem(testItem));

		Handle<Character> owner(new Character());
		Handle<Character> buyer(new Character());
		am_equals(NULL_PARAMETER, store->buyItem(nullptr, nullptr));
		am_equals(NULL_PARAMETER, store->buyItem(nullptr, testItem));
		am_equals(NULL_PARAMETER, store->buyItem(buyer, nullptr));
		am_equals(NO_STORE_OWNER, store->buyItem(buyer, testItem));

		store->setStoreOwner(owner);
		am_equals(NO_INVENTORIES, store->buyItem(buyer, testItem));

		store->addStoreInventory(storeInv);
		am_equals(NOT_ENOUGH_COIN, store->buyItem(buyer, testItem));

		buyer->getCoinPurse()->addCoin(70);
		am_equals(SUCCESS, store->buyItem(buyer, testItem));
		am_equals(20, buyer->getCoinPurse()->getCoin());
		am_equals(50, owner->getCoinPurse()->getCoin());
		am_equals(false, storeInv->hasItem(testItem));
		am_equals(true, buyer->getInventory()->hasItem(testItem));

		return true;
	}

}
}
