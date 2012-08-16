#include "test_character.h"

#include <game/character.h>
#include <game/item.h>
#include <game/stats.h>
#include <game/inventory.h>
#include <game/stat_modifier.h>
#include <game/stat_modifiers.h>
using namespace am::game;

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

}
}
