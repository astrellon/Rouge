#include "test_character.h"

#include <game/character.h>
#include <game/item.h>
#include <game/stats.h>
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

	bool TestCharacter::testSelector()
	{
		Race testRace("human");
		Handle<Character> testChar(new Character());
		testChar->setAge(23.0f);
		testChar->setGender(Gender::FEMALE);
		testChar->setRace(&testRace);

		Selector selector;
		testChar->getSelector(selector);
		equalsDelta(23.0f, selector.getAttribute("age").getFloat(), 0.001f);
		assert(strcmp(Gender::getGenderName(Gender::FEMALE), selector.getAttribute("gender").getCStr()) == 0);
		assert(strcmp("human", selector.getAttribute("race").getCStr()) == 0);

		return true;
	}

}
}
