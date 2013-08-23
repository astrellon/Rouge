#include "test_character.h"

#include <game/character.h>
#include <game/item.h>
#include <game/stats.h>
#include <game/game.h>
#include <game/engine.h>
#include <game/inventory.h>
#include <game/stat_modifier.h>
#include <game/stat_modifiers.h>
#include <game/body_part.h>
#include <game/race.h>
#include <game/gender.h>
using namespace am::game;

#include <util/selector.h>
using namespace am::util;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestCharacter::testSimple() {
		Handle<Character> testChar(new Character());
		testChar->setName("TestChar");
		am_equals("TestChar", testChar->getName().c_str());

		assert(testChar->getInventory());

		Stats *stats = testChar->getStats();
		stats->setBaseStat(Stat::STRENGTH, 10.0f);

		am_equalsDelta(10.0f, stats->getBaseStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(10.0f, stats->getStat(Stat::STRENGTH), 0.0001f);

		Handle<Item> sword(new Item());
		StatModifiers *modifiers = sword->getStatModifiers();
		modifiers->addStatModifier(Stat::STRENGTH, StatModifier(5, MOD_ADD));

		stats->addModifiers(modifiers);

		am_equalsDelta(10.0f, stats->getBaseStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(15.0f, stats->getStat(Stat::STRENGTH), 0.0001f);

		stats->removeModifiers(modifiers);

		modifiers->addStatModifier(Stat::STRENGTH, StatModifier(2.0f, MOD_MULTIPLY));

		stats->addModifiers(modifiers);

		am_equalsDelta(10.0f, stats->getBaseStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(25.0f, stats->getStat(Stat::STRENGTH), 0.0001f);

		return true;
	}

	bool TestCharacter::testEquipped()
	{
		Handle<Character> testChar(new Character());
		testChar->setName("TestChar");
		
		Stats *stats = testChar->getStats();
		stats->setBaseStat(Stat::STRENGTH, 10.0f);

		am_equalsDelta(10.0f, stats->getBaseStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(10.0f, stats->getStat(Stat::STRENGTH), 0.0001f);

		Handle<BodyPart> part(new BodyPart("arm", BodyPartType::ARM));
		assert(testChar->addBodyPart(part));

		Handle<Item> sword(new Item());
		sword->getStatModifiers()->addStatModifier(Stat::STRENGTH, StatModifier(5.0f, MOD_ADD));
		assert(testChar->equipItem(sword, "arm"));

		am_equalsDelta(10.0f, stats->getBaseStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(15.0f, stats->getStat(Stat::STRENGTH), 0.0001f);

		assert(testChar->unequipItem("arm"));

		am_equalsDelta(10.0f, stats->getBaseStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(10.0f, stats->getStat(Stat::STRENGTH), 0.0001f);

		Handle<BodyPart> leftHand(new BodyPart("left_hand", BodyPartType::HAND));
		leftHand->setWeaponPart(true);
		Handle<BodyPart> rightHand(new BodyPart("right_hand", BodyPartType::HAND));
		rightHand->setWeaponPart(true);
		testChar->addBodyPart(leftHand);
		testChar->addBodyPart(rightHand);

		Handle<Item> twoHandSword(new Item());
		twoHandSword->setBodyPartsRequired(2);
		twoHandSword->addBodyPartType(BodyPartType::HAND);

		// Not enough body parts
		am_equals(NOT_ENOUGH_BODY_PARTS, testChar->canEquipItem(twoHandSword, leftHand));
		rightHand->setCanHoldOnto(leftHand);
		am_equals(ABLE_TO_EQUIP, testChar->canEquipItem(twoHandSword, leftHand));

		Handle<Item> dagger(new Item());
		dagger->addBodyPartType(BodyPartType::HAND);
		am_equals(SUCCESS, rightHand->setEquippedItem(dagger));

		am_equals(CAN_EQUIP, testChar->canEquipItem(twoHandSword, leftHand));
		
		return true;
	}

	bool TestCharacter::testCopy()
	{
		Engine *prevEngine = Engine::getEngine();
		Engine *eng = new Engine();
		eng->addRace(new Race("human"));

		Engine::setEngine(eng);
		Handle<Game> game = new Game();
		eng->setCurrentGame(game);
		
		Handle<Character> testChar(new Character());
		testChar->setAge(24);
		testChar->setName("Melli");
		testChar->setCameraOffset(0.8f, 0.4f);
		DialogueComponent *comp = new DialogueComponent(testChar);
		comp->setDialogueAvailable("testSub1");
		comp->setDialogueAvailable("testSub2");
		comp->setSubjectLock("testSub1");
		testChar->setDialogueComp(comp);

		testChar->setFixedToGrid(true);
		testChar->setGender(Gender::FEMALE);
		testChar->setGraphic(new Sprite("characters/human:kaswe"), false);
		testChar->setGridLocation(5, 4);
		testChar->setLevel(2);
		testChar->setMaxLevel(10);
		testChar->setPickupReach(1.4f);
		testChar->setRace(Engine::getEngine()->getRace("human"));

		Handle<Inventory> inv(testChar->getInventory());
		Handle<Item> sword(new Item());
		sword->setGraphic(new Sprite("items/sword:main"));
		sword->setGroundGraphic(new Sprite("items/sword:ground"));
		sword->getStatModifiers()->addStatModifier(Stat::MAX_DAMAGE, StatModifier(5.0f, MOD_ADD));
		sword->getStatModifiers()->addStatModifier(Stat::MIN_DAMAGE, StatModifier(3.0f, MOD_ADD));
		sword->setItemFullname("Sword", "Wooden", "of Death");
		sword->setItemType(ItemCommon::SWORD);
		sword->setInventorySize(2, 3);
		inv->addItem(sword, 0, 0);

		Handle<Item> shield(new Item());
		shield->getStatModifiers()->addStatModifier(Stat::ARMOUR, StatModifier(4.0f, MOD_MULTIPLY));
		shield->getStatModifiers()->addStatModifier(Stat::ARMOUR, StatModifier(2.0f, MOD_ADD));
		shield->setItemFullname("Shield", "Padded", "of ASD");
		shield->setItemType(ItemCommon::SHIELD);
		shield->setInventorySize(2, 2);
		inv->addItem(shield, 4, 2);

		testChar->getStats()->setBaseStat(Stat::HEALTH, 10.0f);
		testChar->getStats()->setBaseStat(Stat::STRENGTH, 5.5f);
		testChar->getStats()->setBaseStat(Stat::MAX_DAMAGE, 4.0f);
		testChar->getStats()->setBaseStat(Stat::MIN_DAMAGE, 4.0f);
		testChar->getStats()->setBaseStat(Stat::ARMOUR, 7.0f);
		
		Handle<Item> swordEquip(new Item(*sword));
		swordEquip->setItemFullname("Sword", "Wooden", "of Hit");
		testChar->addBodyPart(new BodyPart("arm", BodyPartType::ARM, swordEquip));
		testChar->addBodyPart(new BodyPart("torso", BodyPartType::TORSO));
		testChar->addBodyPart(new BodyPart("legs", BodyPartType::LEGS));

		am_equalsDelta(10.0f, testChar->getStats()->getStat(Stat::HEALTH), 0.0001f);
		am_equalsDelta(5.5f, testChar->getStats()->getStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(9.0f, testChar->getStats()->getStat(Stat::MAX_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, testChar->getStats()->getStat(Stat::MIN_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, testChar->getStats()->getStat(Stat::ARMOUR), 0.0001f);

		Handle<Character> copyChar(new Character(*testChar));
		am_equalsDelta(24.0f, copyChar->getAge(), 0.0001f);
		am_equalsStr("Melli", copyChar->getName());
		am_equalsDelta(0.8f, copyChar->getCameraOffsetX(), 0.0001f);
		am_equalsDelta(0.4f, copyChar->getCameraOffsetY(), 0.0001f);

		DialogueComponent *copyComp = copyChar->getDialogueComp();
		assert(copyComp != comp);
		assert(copyComp->getAttachedTo() == copyChar);
		assert(copyComp->isDialogueAvailable("testSub1"));
		assert(copyComp->isDialogueAvailable("testSub2"));
		assert(!copyComp->isSubjectLocked("testSub1"));
		assert(copyComp->isSubjectLocked("testSub2"));

		assert(copyChar->isFixedToGrid());
		assert(copyChar->getGender() == Gender::FEMALE);
		assert(copyChar->getGraphic() != testChar->getGraphic());
		assert(copyChar->getGraphic()->getAsset() == testChar->getGraphic()->getAsset());
		am_equals(5, copyChar->getGridLocationX());
		am_equals(4, copyChar->getGridLocationY());
		am_equals(2, copyChar->getLevel());
		am_equals(10, copyChar->getMaxLevel());
		am_equalsDelta(1.4f, copyChar->getPickupReach(), 0.0001f);
		assert(copyChar->getRace() == Engine::getEngine()->getRace("human"));

		Handle<Inventory> copyInv(copyChar->getInventory());
		assert(copyInv.get() && copyInv != inv);
		
		Handle<Item> copySword(copyInv->getItemAt(0, 0));
		assert(copySword.get() && copySword != sword);
		am_equals(1u, copySword->getStatModifiers()->getModifiers()[Stat::MAX_DAMAGE].size());
		am_equalsDelta(5.0f, copySword->getStatModifiers()->getModifiers()[Stat::MAX_DAMAGE][0].getValue(), 0.0001f);
		assert(MOD_ADD == copySword->getStatModifiers()->getModifiers()[Stat::MAX_DAMAGE][0].getType());
		am_equals(1u, copySword->getStatModifiers()->getModifiers()[Stat::MIN_DAMAGE].size());
		assert(MOD_ADD == copySword->getStatModifiers()->getModifiers()[Stat::MIN_DAMAGE][0].getType());
		am_equalsDelta(3.0f, copySword->getStatModifiers()->getModifiers()[Stat::MIN_DAMAGE][0].getValue(), 0.0001f);
		am_equalsStr("Sword", copySword->getItemName());
		am_equalsStr("Wooden", copySword->getPrefix());
		am_equalsStr("of Death", copySword->getPostfix());
		am_equalsStr("Wooden Sword of Death", copySword->getFullItemName());
		assert(ItemCommon::SWORD == copySword->getItemType());
		am_equals(2, copySword->getInventorySizeX());
		am_equals(3, copySword->getInventorySizeY());

		assert(copySword->getGraphic() != sword->getGraphic());
		assert(copySword->getGraphic()->getAsset() == sword->getGraphic()->getAsset());

		assert(copySword->getGroundGraphic() != sword->getGroundGraphic());
		assert(copySword->getGroundGraphic()->getAsset() == sword->getGroundGraphic()->getAsset());

		Handle<Item> copyShield(copyInv->getItemAt(4, 2));
		assert(copyShield.get() && copyShield != shield);
		am_equals(2u, copyShield->getStatModifiers()->getModifiers()[Stat::ARMOUR].size());
		am_equalsDelta(4.0f, copyShield->getStatModifiers()->getModifiers()[Stat::ARMOUR][0].getValue(), 0.0001f);
		assert(MOD_MULTIPLY == copyShield->getStatModifiers()->getModifiers()[Stat::ARMOUR][0].getType());
		am_equalsDelta(2.0f, copyShield->getStatModifiers()->getModifiers()[Stat::ARMOUR][1].getValue(), 0.0001f);
		assert(MOD_ADD == copyShield->getStatModifiers()->getModifiers()[Stat::ARMOUR][1].getType());

		am_equalsStr("Shield", copyShield->getItemName());
		am_equalsStr("Padded", copyShield->getPrefix());
		am_equalsStr("of ASD", copyShield->getPostfix());
		am_equalsStr("Padded Shield of ASD", copyShield->getFullItemName());
		assert(ItemCommon::SHIELD == copyShield->getItemType());
		am_equals(2, copyShield->getInventorySizeX());
		am_equals(2, copyShield->getInventorySizeY());

		am_equalsDelta(10.0f, testChar->getStats()->getBaseStat(Stat::HEALTH), 0.0001f);
		am_equalsDelta(5.5f, testChar->getStats()->getBaseStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(4.0f, testChar->getStats()->getBaseStat(Stat::MAX_DAMAGE), 0.0001f);
		am_equalsDelta(4.0f, testChar->getStats()->getBaseStat(Stat::MIN_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, testChar->getStats()->getBaseStat(Stat::ARMOUR), 0.0001f);

		const BodyParts &parts = copyChar->getBodyParts();
		assert(parts.hasBodyPart("arm"));
		assert(parts.hasBodyPart("torso"));
		assert(parts.hasBodyPart("legs"));
		Handle<Item> swordEquipCopy(parts.getBodyPart("arm")->getEquippedItem());
		assert(swordEquipCopy.get() && swordEquipCopy != swordEquip);
		am_equalsStr("Wooden Sword of Hit", swordEquipCopy->getFullItemName());

		am_equalsDelta(10.0f, copyChar->getStats()->getStat(Stat::HEALTH), 0.0001f);
		am_equalsDelta(5.5f, copyChar->getStats()->getStat(Stat::STRENGTH), 0.0001f);
		am_equalsDelta(9.0f, copyChar->getStats()->getStat(Stat::MAX_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, copyChar->getStats()->getStat(Stat::MIN_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, copyChar->getStats()->getStat(Stat::ARMOUR), 0.0001f);

		Engine::setEngine(prevEngine);
		delete eng;
		
		return true;
	}

	bool TestCharacter::testDead()
	{
		Engine *prevEngine = Engine::getEngine();
		Engine *eng = new Engine();
		eng->addRace(new Race("human"));

		Engine::setEngine(eng);
		Handle<Game> game = new Game();
		eng->setCurrentGame(game);

		Handle<Sprite> deadGraphic(new Sprite());
		Handle<Sprite> aliveGraphic(new Sprite());
		game->setGenericDeadGraphic(deadGraphic);
		
		Handle<Character> testChar(new Character());
		testChar->setGraphic(aliveGraphic);
		assert(testChar->getCharacterLayer()->getChildAt(0) != aliveGraphic);

		assert(testChar->isDead());
		testChar->getStats()->setBaseStat(Stat::MAX_HEALTH, 10.0f);
		testChar->getStats()->setBaseStat(Stat::HEALTH, 10.0f);
		assert(!testChar->isDead());
		testChar->updateGraphic();

		assert(testChar->getCharacterLayer()->getChildAt(0) == aliveGraphic);

		testChar->getStats()->setBaseStat(Stat::HEALTH, 0.0f);
		testChar->updateGraphic();
		assert(testChar->getCharacterLayer()->getChildAt(0) != aliveGraphic);

		Engine::setEngine(prevEngine);
		
		delete eng;
		
		return true;
	}

	/*bool TestCharacter::testSelector()
	{
		Race testRace("human");
		Handle<Character> testChar(new Character());
		testChar->setAge(23.0f);
		testChar->setGender(Gender::FEMALE);
		testChar->setRace(&testRace);

		Selector selector;
		testChar->getSelector(selector);
		am_equalsDelta(23.0f, selector.getAttribute("age").getFloat(), 0.001f);
		am_equalsStr(Gender::getGenderName(Gender::FEMALE), selector.getAttribute("gender").getCStr());
		am_equalsStr("human", selector.getAttribute("race").getCStr());

		return true;
	}*/

}
}
