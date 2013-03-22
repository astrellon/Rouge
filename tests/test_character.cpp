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
		equals("TestChar", testChar->getName().c_str());

		assert(testChar->getInventory());

		Stats &stats = testChar->getStats();
		stats.setBaseStat(Stat::STRENGTH, 10.0f);

		equalsDelta(10.0f, stats.getBaseStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(10.0f, stats.getStat(Stat::STRENGTH), 0.0001f);

		Handle<Item> sword(new Item());
		StatModifiers &modifiers = sword->getStatModifiers();
		modifiers.addStatModifier(Stat::STRENGTH, StatModifier(5, MOD_ADD));

		stats.addModifiers(modifiers);

		equalsDelta(10.0f, stats.getBaseStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(15.0f, stats.getStat(Stat::STRENGTH), 0.0001f);

		stats.removeModifiers(modifiers);

		modifiers.addStatModifier(Stat::STRENGTH, StatModifier(2.0f, MOD_MULTIPLY));

		stats.addModifiers(modifiers);

		equalsDelta(10.0f, stats.getBaseStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(25.0f, stats.getStat(Stat::STRENGTH), 0.0001f);

		return true;
	}

	bool TestCharacter::testEquipped()
	{
		Handle<Character> testChar(new Character());
		testChar->setName("TestChar");
		
		Stats &stats = testChar->getStats();
		stats.setBaseStat(Stat::STRENGTH, 10.0f);

		equalsDelta(10.0f, stats.getBaseStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(10.0f, stats.getStat(Stat::STRENGTH), 0.0001f);

		BodyPart *part = new BodyPart("arm");
		assert(testChar->addBodyPart(part));

		Handle<Item> sword(new Item());
		sword->getStatModifiers().addStatModifier(Stat::STRENGTH, StatModifier(5.0f, MOD_ADD));
		assert(testChar->equipItem(sword, "arm"));

		equalsDelta(10.0f, stats.getBaseStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(15.0f, stats.getStat(Stat::STRENGTH), 0.0001f);

		assert(testChar->unequipItem("arm"));

		equalsDelta(10.0f, stats.getBaseStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(10.0f, stats.getStat(Stat::STRENGTH), 0.0001f);

		return true;
	}

	bool TestCharacter::testCopy()
	{
		Engine *prevEngine = Engine::getEngine();
		Engine *eng = new Engine();
		eng->addRace(new Race("human"));

		Engine::setEngine(eng);
		Handle<Game> game = new Game();
		
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
		testChar->setGraphic(new Sprite("characters/mainChar/front"), false);
		testChar->setGridLocation(5, 4);
		testChar->setLevel(2);
		testChar->setMaxLevel(10);
		testChar->setPickupReach(1.4f);
		testChar->setRace(Engine::getEngine()->getRace("human"));

		Handle<Inventory> inv(testChar->getInventory());
		Handle<Item> sword(new Item());
		sword->setGraphic(new Sprite("items/sword"));
		sword->setGroundGraphic(new Sprite("items/swordGround"));
		sword->getStatModifiers().addStatModifier(Stat::MAX_DAMAGE, StatModifier(5.0f, MOD_ADD));
		sword->getStatModifiers().addStatModifier(Stat::MIN_DAMAGE, StatModifier(3.0f, MOD_ADD));
		sword->setItemFullname("Sword", "Wooden", "of Death");
		sword->setItemType(ItemCommon::SWORD);
		sword->setInventorySize(2, 3);
		inv->addItem(sword, 0, 0);

		Handle<Item> shield(new Item());
		shield->getStatModifiers().addStatModifier(Stat::ARMOUR, StatModifier(4.0f, MOD_MULTIPLY));
		shield->getStatModifiers().addStatModifier(Stat::ARMOUR, StatModifier(2.0f, MOD_ADD));
		shield->setItemFullname("Shield", "Padded", "of ASD");
		shield->setItemType(ItemCommon::SHIELD);
		shield->setInventorySize(2, 2);
		inv->addItem(shield, 4, 2);

		testChar->getStats().setBaseStat(Stat::HEALTH, 10.0f);
		testChar->getStats().setBaseStat(Stat::STRENGTH, 5.5f);
		testChar->getStats().setBaseStat(Stat::MAX_DAMAGE, 4.0f);
		testChar->getStats().setBaseStat(Stat::MIN_DAMAGE, 4.0f);
		testChar->getStats().setBaseStat(Stat::ARMOUR, 7.0f);
		
		Handle<Item> swordEquip(new Item(*sword));
		swordEquip->setItemFullname("Sword", "Wooden", "of Hit");
		testChar->addBodyPart(new BodyPart("arm", swordEquip));
		testChar->addBodyPart(new BodyPart("torso"));
		testChar->addBodyPart(new BodyPart("legs"));

		equalsDelta(10.0f, testChar->getStats().getStat(Stat::HEALTH), 0.0001f);
		equalsDelta(5.5f, testChar->getStats().getStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(9.0f, testChar->getStats().getStat(Stat::MAX_DAMAGE), 0.0001f);
		equalsDelta(7.0f, testChar->getStats().getStat(Stat::MIN_DAMAGE), 0.0001f);
		equalsDelta(7.0f, testChar->getStats().getStat(Stat::ARMOUR), 0.0001f);

		Handle<Character> copyChar(new Character(*testChar));
		equalsDelta(24.0f, copyChar->getAge(), 0.0001f);
		equalsStr("Melli", copyChar->getName());
		equalsDelta(0.8f, copyChar->getCameraOffsetX(), 0.0001f);
		equalsDelta(0.4f, copyChar->getCameraOffsetY(), 0.0001f);

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
		equals(5, copyChar->getGridLocationX());
		equals(4, copyChar->getGridLocationY());
		equals(2, copyChar->getLevel());
		equals(10, copyChar->getMaxLevel());
		equalsDelta(1.4f, copyChar->getPickupReach(), 0.0001f);
		assert(copyChar->getRace() == Engine::getEngine()->getRace("human"));

		Handle<Inventory> copyInv(copyChar->getInventory());
		assert(copyInv.get() && copyInv != inv);
		
		Handle<Item> copySword(copyInv->getItemAt(0, 0));
		assert(copySword.get() && copySword != sword);
		equals(1u, copySword->getStatModifiers().getModifiers()[Stat::MAX_DAMAGE].size());
		equalsDelta(5.0f, copySword->getStatModifiers().getModifiers()[Stat::MAX_DAMAGE][0].getValue(), 0.0001f);
		assert(MOD_ADD == copySword->getStatModifiers().getModifiers()[Stat::MAX_DAMAGE][0].getType());
		equals(1u, copySword->getStatModifiers().getModifiers()[Stat::MIN_DAMAGE].size());
		assert(MOD_ADD == copySword->getStatModifiers().getModifiers()[Stat::MIN_DAMAGE][0].getType());
		equalsDelta(3.0f, copySword->getStatModifiers().getModifiers()[Stat::MIN_DAMAGE][0].getValue(), 0.0001f);
		equalsStr("Sword", copySword->getItemName());
		equalsStr("Wooden", copySword->getPrefix());
		equalsStr("of Death", copySword->getPostfix());
		equalsStr("Wooden Sword of Death", copySword->getFullItemName());
		assert(ItemCommon::SWORD == copySword->getItemType());
		assert(2, copySword->getInventorySizeX());
		assert(3, copySword->getInventorySizeY());

		assert(copySword->getGraphic() != sword->getGraphic());
		assert(copySword->getGraphic()->getAsset() == sword->getGraphic()->getAsset());

		assert(copySword->getGroundGraphic() != sword->getGroundGraphic());
		assert(copySword->getGroundGraphic()->getAsset() == sword->getGroundGraphic()->getAsset());

		Handle<Item> copyShield(copyInv->getItemAt(4, 2));
		assert(copyShield.get() && copyShield != shield);
		equals(2u, copyShield->getStatModifiers().getModifiers()[Stat::ARMOUR].size());
		equalsDelta(4.0f, copyShield->getStatModifiers().getModifiers()[Stat::ARMOUR][0].getValue(), 0.0001f);
		assert(MOD_MULTIPLY == copyShield->getStatModifiers().getModifiers()[Stat::ARMOUR][0].getType());
		equalsDelta(2.0f, copyShield->getStatModifiers().getModifiers()[Stat::ARMOUR][1].getValue(), 0.0001f);
		assert(MOD_ADD == copyShield->getStatModifiers().getModifiers()[Stat::ARMOUR][1].getType());

		equalsStr("Shield", copyShield->getItemName());
		equalsStr("Padded", copyShield->getPrefix());
		equalsStr("of ASD", copyShield->getPostfix());
		equalsStr("Padded Shield of ASD", copyShield->getFullItemName());
		assert(ItemCommon::SHIELD == copyShield->getItemType());
		equals(2, copyShield->getInventorySizeX());
		equals(2, copyShield->getInventorySizeY());

		equalsDelta(10.0f, testChar->getStats().getBaseStat(Stat::HEALTH), 0.0001f);
		equalsDelta(5.5f, testChar->getStats().getBaseStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(4.0f, testChar->getStats().getBaseStat(Stat::MAX_DAMAGE), 0.0001f);
		equalsDelta(4.0f, testChar->getStats().getBaseStat(Stat::MIN_DAMAGE), 0.0001f);
		equalsDelta(7.0f, testChar->getStats().getBaseStat(Stat::ARMOUR), 0.0001f);

		const BodyPart::BodyPartMap &parts = copyChar->getBodyParts();
		assert(parts.find(string("arm")) != parts.end());
		assert(parts.find(string("torso")) != parts.end());
		assert(parts.find(string("legs")) != parts.end());
		Handle<Item> swordEquipCopy(parts.at(string("arm"))->getEqippedItem());
		assert(swordEquipCopy.get() && swordEquipCopy != swordEquip);
		equalsStr("Wooden Sword of Hit", swordEquipCopy->getFullItemName());

		equalsDelta(10.0f, copyChar->getStats().getStat(Stat::HEALTH), 0.0001f);
		equalsDelta(5.5f, copyChar->getStats().getStat(Stat::STRENGTH), 0.0001f);
		equalsDelta(9.0f, copyChar->getStats().getStat(Stat::MAX_DAMAGE), 0.0001f);
		equalsDelta(7.0f, copyChar->getStats().getStat(Stat::MIN_DAMAGE), 0.0001f);
		equalsDelta(7.0f, copyChar->getStats().getStat(Stat::ARMOUR), 0.0001f);

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
		equalsDelta(23.0f, selector.getAttribute("age").getFloat(), 0.001f);
		equalsStr(Gender::getGenderName(Gender::FEMALE), selector.getAttribute("gender").getCStr());
		equalsStr("human", selector.getAttribute("race").getCStr());

		return true;
	}*/

}
}
