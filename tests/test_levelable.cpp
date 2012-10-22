#include "test_levelable.h"

#include <game/levelable.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestLevelable::testSimple()
	{
		Levelable level;
		equals(0, level.getLevel());
		equals(0, level.getMaxLevel());
		equals(0, level.getExperience());

		equals(1000, level.getExperienceForLevel(1));
		equals(2000, level.getExperienceForLevel(2));
		equals(4000, level.getExperienceForLevel(4));

		equals(1, level.getLevelFromExperience(1000));
		equals(2, level.getLevelFromExperience(2000));
		equals(4, level.getLevelFromExperience(4000));

		level.addExperience(100);
		equals(0, level.getLevel());
		equals(100, level.getExperience());

		level.addExperience(100);
		equals(0, level.getLevel());
		equals(200, level.getExperience());
		
		level.setExperience(500);
		equals(0, level.getLevel());
		equals(500, level.getExperience());

		level.addExperience(500);
		equals(1, level.getLevel());
		equals(1000, level.getExperience());

		level.addExperience(1050);
		equals(2, level.getLevel());
		equals(2050, level.getExperience());

		return true;
	}

	bool TestLevelable::testMaxLevel()
	{
		Levelable level;
		level.setMaxLevel(4);
		equals(0, level.getLevel());
		equals(4, level.getMaxLevel());
		equals(0, level.getExperience());

		level.addExperience(1000);
		equals(1, level.getLevel());
		equals(4, level.getMaxLevel());
		equals(1000, level.getExperience());

		level.addExperience(2000);
		equals(3, level.getLevel());
		equals(4, level.getMaxLevel());
		equals(3000, level.getExperience());

		level.addExperience(2000);
		equals(4, level.getLevel());
		equals(4, level.getMaxLevel());
		equals(4000, level.getExperience());

		level.addExperience(2000);
		equals(4, level.getLevel());
		equals(4, level.getMaxLevel());
		equals(4000, level.getExperience());

		return true;
	}
}
}
