#include "test_store.h"

#include <game/store.h>
#include <game/character.h>
#include <game/inventory.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestStore::testBuy()
	{
		am::base::Handle<Store> store(new Store());
		am::base::Handle<Inventory> storeInv(new Inventory(10, 10));
		am::base::Handle<Item> testItem(new Item());
		testItem->setInventorySize(2, 2);
		testItem->setItemValue(50);
		storeInv->addItem(testItem);
		am_equals(true, storeInv->hasItem(testItem));

		am::base::Handle<Character> owner(new Character());
		am::base::Handle<Character> buyer(new Character());
		am_equals(am::base::NULL_PARAMETER, store->buyItem(nullptr, nullptr));
		am_equals(am::base::NULL_PARAMETER, store->buyItem(nullptr, testItem));
		am_equals(am::base::NULL_PARAMETER, store->buyItem(buyer, nullptr));
		am_equals(am::base::NO_STORE_OWNER, store->buyItem(buyer, testItem));

		store->setStoreOwner(owner);
		am_equals(am::base::NO_INVENTORIES, store->buyItem(buyer, testItem));

		store->addStoreInventory(storeInv);
		am_equals(am::base::NOT_ENOUGH_COIN, store->buyItem(buyer, testItem));

		buyer->getCoinPurse()->addCoin(70);
		am_equals(am::base::SUCCESS, store->buyItem(buyer, testItem));
		am_equals(20, buyer->getCoinPurse()->getCoin());
		am_equals(50, owner->getCoinPurse()->getCoin());
		am_equals(false, storeInv->hasItem(testItem));
		am_equals(false, buyer->getInventory()->hasItem(testItem));

		buyer->getInventory()->addItem(testItem);
		am_equals(true, buyer->getInventory()->hasItem(testItem));

		return true;
	}

	bool TestStore::testSell()
	{
		am::base::Handle<Store> store(new Store());
		am::base::Handle<Item> testItem(new Item());
		testItem->setInventorySize(2, 2);
		testItem->setItemValue(50);
		
		am::base::Handle<Character> owner(new Character());
		am::base::Handle<Character> seller(new Character());
		am_equals(am::base::NULL_PARAMETER, store->sellItem(nullptr, nullptr));
		am_equals(am::base::NULL_PARAMETER, store->sellItem(nullptr, testItem));
		am_equals(am::base::NULL_PARAMETER, store->sellItem(seller, nullptr));
		am_equals(am::base::NO_STORE_OWNER, store->sellItem(seller, testItem));

		store->setStoreOwner(owner);
		am_equals(am::base::NO_INVENTORIES, store->sellItem(seller, testItem));
		am_equals(am::base::NOT_ENOUGH_COIN, store->sellItem(seller, testItem, true));
		am_equals(0u, store->getStoreInventories().size());
		am_equals(am::base::SUCCESS, store->sellItem(seller, testItem, true, true));
		am_equals(1u, store->getStoreInventories().size());
		am_equals(true, store->getStoreInventories()[0]->hasItem(testItem));
		
		return true;
	}

	bool TestStore::testSell2()
	{
		am::base::Handle<Store> store(new Store());
		store->setDefaultInventorySize(3, 2);
		am::base::Handle<Inventory> storeInv(store->createStoreInventory());
		am::base::Handle<Item> testItem(new Item());
		testItem->setInventorySize(2, 2);
		testItem->setItemValue(50);
		storeInv->addItem(testItem);

		am::base::Handle<Character> owner(new Character());
		am::base::Handle<Character> seller(new Character());
		store->setStoreOwner(owner);
		owner->getCoinPurse()->setCoin(80);
		
		am::base::Handle<Item> testItem2(new Item(*testItem));
		am_equals(1u, store->getStoreInventories().size());
		am_equals(am::base::NOT_ENOUGH_INVENTORY_SPACE, store->sellItem(seller, testItem2));
		am_equals(1u, store->getStoreInventories().size());
		am_equals(am::base::SUCCESS, store->sellItem(seller, testItem2, true));
		am_equals(2u, store->getStoreInventories().size());
		am_equals(30, owner->getCoinPurse()->getCoin());
		am_equals(50, seller->getCoinPurse()->getCoin());
		
		return true;
	}

}
}
