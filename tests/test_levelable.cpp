#include "test_levelable.h"

#include <game/levelable.h>
using namespace am::game;

#include <tests/asserts.h>

namespace am {
namespace tests {

	bool TestLevelable::testSimple()
	{
		game::Levelable level;
		am_equals(0, level.getLevel());
		am_equals(0, level.getMaxLevel());
		am_equals(0, level.getExperience());

		am_equals(1000, level.getExperienceForLevel(1));
		am_equals(2000, level.getExperienceForLevel(2));
		am_equals(4000, level.getExperienceForLevel(4));

		am_equals(1, level.getLevelFromExperience(1000));
		am_equals(2, level.getLevelFromExperience(2000));
		am_equals(4, level.getLevelFromExperience(4000));

		level.addExperience(100);
		am_equals(0, level.getLevel());
		am_equals(100, level.getExperience());

		level.addExperience(100);
		am_equals(0, level.getLevel());
		am_equals(200, level.getExperience());
		
		level.setExperience(500);
		am_equals(0, level.getLevel());
		am_equals(500, level.getExperience());

		level.addExperience(500);
		am_equals(1, level.getLevel());
		am_equals(1000, level.getExperience());

		level.addExperience(1050);
		am_equals(2, level.getLevel());
		am_equals(2050, level.getExperience());

		return true;
	}

	bool TestLevelable::testMaxLevel()
	{
		game::Levelable level;
		level.setMaxLevel(4);
		am_equals(0, level.getLevel());
		am_equals(4, level.getMaxLevel());
		am_equals(0, level.getExperience());

		level.addExperience(1000);
		am_equals(1, level.getLevel());
		am_equals(4, level.getMaxLevel());
		am_equals(1000, level.getExperience());

		level.addExperience(2000);
		am_equals(3, level.getLevel());
		am_equals(4, level.getMaxLevel());
		am_equals(3000, level.getExperience());

		level.addExperience(2000);
		am_equals(4, level.getLevel());
		am_equals(4, level.getMaxLevel());
		am_equals(4000, level.getExperience());

		level.addExperience(2000);
		am_equals(4, level.getLevel());
		am_equals(4, level.getMaxLevel());
		am_equals(4000, level.getExperience());

		return true;
	}
}
}
