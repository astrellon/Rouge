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
#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestCharacter::testSimple() {
		base::Handle<game::Character> testChar(new game::Character());
		testChar->setName("TestChar");
		am_equals("TestChar", testChar->getName().c_str());

		assert(testChar->getInventory());

		game::Stats *stats = testChar->getStats();
		stats->setBaseStat(game::Stat::STRENGTH, 10.0f);

		am_equalsDelta(10.0f, stats->getBaseStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(10.0f, stats->getStat(game::Stat::STRENGTH), 0.0001f);

		base::Handle<game::Item> sword(new game::Item());
		StatModifiers *modifiers = sword->getStatModifiers();
		modifiers->addStatModifier(game::Stat::STRENGTH, game::StatModifier(5, MOD_ADD));

		stats->addModifiers(modifiers);

		am_equalsDelta(10.0f, stats->getBaseStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(15.0f, stats->getStat(game::Stat::STRENGTH), 0.0001f);

		stats->removeModifiers(modifiers);

		modifiers->addStatModifier(game::Stat::STRENGTH, game::StatModifier(2.0f, MOD_MULTIPLY));

		stats->addModifiers(modifiers);

		am_equalsDelta(10.0f, stats->getBaseStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(25.0f, stats->getStat(game::Stat::STRENGTH), 0.0001f);

		return true;
	}

	bool TestCharacter::testEquipped()
	{
		base::Handle<game::Character> testChar(new game::Character());
		testChar->setName("TestChar");
		
		game::Stats *stats = testChar->getStats();
		stats->setBaseStat(game::Stat::STRENGTH, 10.0f);

		am_equalsDelta(10.0f, stats->getBaseStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(10.0f, stats->getStat(game::Stat::STRENGTH), 0.0001f);

		base::Handle<game::BodyPart> part(new game::BodyPart("arm", game::BodyPartType::ARM));
		assert(testChar->addBodyPart(part));

		base::Handle<game::Item> sword(new game::Item());
		sword->getStatModifiers()->addStatModifier(game::Stat::STRENGTH, game::StatModifier(5.0f, MOD_ADD));
		assert(testChar->equipItem(sword, "arm"));

		am_equalsDelta(10.0f, stats->getBaseStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(15.0f, stats->getStat(game::Stat::STRENGTH), 0.0001f);

		assert(testChar->unequipItem("arm"));

		am_equalsDelta(10.0f, stats->getBaseStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(10.0f, stats->getStat(game::Stat::STRENGTH), 0.0001f);

		base::Handle<game::BodyPart> leftHand(new game::BodyPart("left_hand", game::BodyPartType::HAND));
		leftHand->setWeaponPart(true);
		base::Handle<game::BodyPart> rightHand(new game::BodyPart("right_hand", game::BodyPartType::HAND));
		rightHand->setWeaponPart(true);
		testChar->addBodyPart(leftHand);
		testChar->addBodyPart(rightHand);

		base::Handle<game::Item> twoHandSword(new game::Item());
		twoHandSword->setBodyPartsRequired(2);
		twoHandSword->addBodyPartType(game::BodyPartType::HAND);

		// Not enough body parts
		am_equals(base::NOT_ENOUGH_BODY_PARTS, testChar->canEquipItem(twoHandSword, leftHand));
		rightHand->setCanHoldOnto(leftHand);
		am_equals(base::ABLE_TO_EQUIP, testChar->canEquipItem(twoHandSword, leftHand));

		base::Handle<game::Item> dagger(new game::Item());
		dagger->addBodyPartType(game::BodyPartType::HAND);
		am_equals(base::SUCCESS, rightHand->setEquippedItem(dagger));

		am_equals(base::CAN_EQUIP, testChar->canEquipItem(twoHandSword, leftHand));
		
		return true;
	}

	bool TestCharacter::testCopy()
	{
		game::Engine *prevEngine = game::Engine::getEngine();
		game::Engine *eng = new game::Engine();
		eng->addRace(new Race("human"));

		game::Engine::setEngine(eng);
		base::Handle<game::Game> game = new game::Game();
		eng->setCurrentGame(game);
		
		base::Handle<game::Character> testChar(new game::Character());
		testChar->setAge(24);
		testChar->setName("Melli");
		testChar->setCameraOffset(0.8f, 0.4f);
		game::DialogueComponent *comp = new game::DialogueComponent(testChar);
		comp->setDialogueAvailable("testSub1");
		comp->setDialogueAvailable("testSub2");
		comp->setSubjectLock("testSub1");
		testChar->setDialogueComp(comp);

		testChar->setFixedToGrid(true);
		testChar->setGender(Gender::FEMALE);
		testChar->setGraphic(new gfx::Sprite("characters/human:kaswe"), false);
		testChar->setGridLocation(5, 4);
		testChar->setLevel(2);
		testChar->setMaxLevel(10);
		testChar->setPickupReach(1.4f);
		testChar->setRace(game::Engine::getEngine()->getRace("human"));

		base::Handle<Inventory> inv(testChar->getInventory());
		base::Handle<game::Item> sword(new game::Item());
		sword->setGraphic(new gfx::Sprite("items/sword:main"));
		sword->setGroundGraphic(new gfx::Sprite("items/sword:ground"));
		sword->getStatModifiers()->addStatModifier(game::Stat::MAX_DAMAGE, game::StatModifier(5.0f, MOD_ADD));
		sword->getStatModifiers()->addStatModifier(game::Stat::MIN_DAMAGE, game::StatModifier(3.0f, MOD_ADD));
		sword->setItemFullname("Sword", "Wooden", "of Death");
		sword->setItemType(ItemCommon::SWORD);
		sword->setInventorySize(2, 3);
		inv->addItem(sword, 0, 0);

		base::Handle<game::Item> shield(new game::Item());
		shield->getStatModifiers()->addStatModifier(game::Stat::ARMOUR, game::StatModifier(4.0f, MOD_MULTIPLY));
		shield->getStatModifiers()->addStatModifier(game::Stat::ARMOUR, game::StatModifier(2.0f, MOD_ADD));
		shield->setItemFullname("Shield", "Padded", "of ASD");
		shield->setItemType(ItemCommon::SHIELD);
		shield->setInventorySize(2, 2);
		inv->addItem(shield, 4, 2);

		testChar->getStats()->setBaseStat(game::Stat::HEALTH, 10.0f);
		testChar->getStats()->setBaseStat(game::Stat::STRENGTH, 5.5f);
		testChar->getStats()->setBaseStat(game::Stat::MAX_DAMAGE, 4.0f);
		testChar->getStats()->setBaseStat(game::Stat::MIN_DAMAGE, 4.0f);
		testChar->getStats()->setBaseStat(game::Stat::ARMOUR, 7.0f);
		
		base::Handle<game::Item> swordEquip(new game::Item(*sword));
		swordEquip->setItemFullname("Sword", "Wooden", "of Hit");
		testChar->addBodyPart(new game::BodyPart("arm", game::BodyPartType::ARM, swordEquip));
		testChar->addBodyPart(new game::BodyPart("torso", game::BodyPartType::TORSO));
		testChar->addBodyPart(new game::BodyPart("legs", game::BodyPartType::LEGS));

		am_equalsDelta(10.0f, testChar->getStats()->getStat(game::Stat::HEALTH), 0.0001f);
		am_equalsDelta(5.5f, testChar->getStats()->getStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(9.0f, testChar->getStats()->getStat(game::Stat::MAX_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, testChar->getStats()->getStat(game::Stat::MIN_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, testChar->getStats()->getStat(game::Stat::ARMOUR), 0.0001f);

		base::Handle<game::Character> copyChar(new game::Character(*testChar));
		am_equalsDelta(24.0f, copyChar->getAge(), 0.0001f);
		am_equalsStr("Melli", copyChar->getName());
		am_equalsDelta(0.8f, copyChar->getCameraOffsetX(), 0.0001f);
		am_equalsDelta(0.4f, copyChar->getCameraOffsetY(), 0.0001f);

		game::DialogueComponent *copyComp = copyChar->getDialogueComp();
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
		assert(copyChar->getRace() == game::Engine::getEngine()->getRace("human"));

		base::Handle<Inventory> copyInv(copyChar->getInventory());
		assert(copyInv.get() && copyInv != inv);
		
		base::Handle<game::Item> copySword(copyInv->getItemAt(0, 0));
		assert(copySword.get() && copySword != sword);
		am_equals(1u, copySword->getStatModifiers()->getModifiers()[game::Stat::MAX_DAMAGE].size());
		am_equalsDelta(5.0f, copySword->getStatModifiers()->getModifiers()[game::Stat::MAX_DAMAGE][0].getValue(), 0.0001f);
		assert(MOD_ADD == copySword->getStatModifiers()->getModifiers()[game::Stat::MAX_DAMAGE][0].getType());
		am_equals(1u, copySword->getStatModifiers()->getModifiers()[game::Stat::MIN_DAMAGE].size());
		assert(MOD_ADD == copySword->getStatModifiers()->getModifiers()[game::Stat::MIN_DAMAGE][0].getType());
		am_equalsDelta(3.0f, copySword->getStatModifiers()->getModifiers()[game::Stat::MIN_DAMAGE][0].getValue(), 0.0001f);
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

		base::Handle<game::Item> copyShield(copyInv->getItemAt(4, 2));
		assert(copyShield.get() && copyShield != shield);
		am_equals(2u, copyShield->getStatModifiers()->getModifiers()[game::Stat::ARMOUR].size());
		am_equalsDelta(4.0f, copyShield->getStatModifiers()->getModifiers()[game::Stat::ARMOUR][0].getValue(), 0.0001f);
		assert(MOD_MULTIPLY == copyShield->getStatModifiers()->getModifiers()[game::Stat::ARMOUR][0].getType());
		am_equalsDelta(2.0f, copyShield->getStatModifiers()->getModifiers()[game::Stat::ARMOUR][1].getValue(), 0.0001f);
		assert(MOD_ADD == copyShield->getStatModifiers()->getModifiers()[game::Stat::ARMOUR][1].getType());

		am_equalsStr("Shield", copyShield->getItemName());
		am_equalsStr("Padded", copyShield->getPrefix());
		am_equalsStr("of ASD", copyShield->getPostfix());
		am_equalsStr("Padded Shield of ASD", copyShield->getFullItemName());
		assert(ItemCommon::SHIELD == copyShield->getItemType());
		am_equals(2, copyShield->getInventorySizeX());
		am_equals(2, copyShield->getInventorySizeY());

		am_equalsDelta(10.0f, testChar->getStats()->getBaseStat(game::Stat::HEALTH), 0.0001f);
		am_equalsDelta(5.5f, testChar->getStats()->getBaseStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(4.0f, testChar->getStats()->getBaseStat(game::Stat::MAX_DAMAGE), 0.0001f);
		am_equalsDelta(4.0f, testChar->getStats()->getBaseStat(game::Stat::MIN_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, testChar->getStats()->getBaseStat(game::Stat::ARMOUR), 0.0001f);

		const game::BodyParts &parts = copyChar->getBodyParts();
		assert(parts.hasBodyPart("arm"));
		assert(parts.hasBodyPart("torso"));
		assert(parts.hasBodyPart("legs"));
		base::Handle<game::Item> swordEquipCopy(parts.getBodyPart("arm")->getEquippedItem());
		assert(swordEquipCopy.get() && swordEquipCopy != swordEquip);
		am_equalsStr("Wooden Sword of Hit", swordEquipCopy->getFullItemName());

		am_equalsDelta(10.0f, copyChar->getStats()->getStat(game::Stat::HEALTH), 0.0001f);
		am_equalsDelta(5.5f, copyChar->getStats()->getStat(game::Stat::STRENGTH), 0.0001f);
		am_equalsDelta(9.0f, copyChar->getStats()->getStat(game::Stat::MAX_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, copyChar->getStats()->getStat(game::Stat::MIN_DAMAGE), 0.0001f);
		am_equalsDelta(7.0f, copyChar->getStats()->getStat(game::Stat::ARMOUR), 0.0001f);

		game::Engine::setEngine(prevEngine);
		delete eng;
		
		return true;
	}

	bool TestCharacter::testDead()
	{
		game::Engine *prevEngine = game::Engine::getEngine();
		game::Engine *eng = new game::Engine();
		eng->addRace(new Race("human"));

		game::Engine::setEngine(eng);
		base::Handle<game::Game> game = new game::Game();
		eng->setCurrentGame(game);

		base::Handle<gfx::Sprite> deadGraphic(new gfx::Sprite());
		base::Handle<gfx::Sprite> aliveGraphic(new gfx::Sprite());
		game->setGenericDeadGraphic(deadGraphic);
		
		base::Handle<game::Character> testChar(new game::Character());
		testChar->setGraphic(aliveGraphic);
		assert(testChar->getCharacterLayer()->getChildAt(0) != aliveGraphic);

		assert(testChar->isDead());
		testChar->getStats()->setBaseStat(game::Stat::MAX_HEALTH, 10.0f);
		testChar->getStats()->setBaseStat(game::Stat::HEALTH, 10.0f);
		assert(!testChar->isDead());
		testChar->updateGraphic();

		assert(testChar->getCharacterLayer()->getChildAt(0) == aliveGraphic);

		testChar->getStats()->setBaseStat(game::Stat::HEALTH, 0.0f);
		testChar->updateGraphic();
		assert(testChar->getCharacterLayer()->getChildAt(0) != aliveGraphic);

		game::Engine::setEngine(prevEngine);
		
		delete eng;
		
		return true;
	}

}
}
