#include "levelable.h"

namespace am {
namespace game {

	Levelable::Levelable() :
		mLevel(0),
		mMaxLevel(0),
		mExperience(0)
	{
	}

	Levelable::~Levelable()
	{
	}

	void Levelable::addExperience(int experience)
	{
		setExperience(mExperience + experience);
	}
	void Levelable::setExperience(int experience)
	{
		int level = getLevelFromExperience(experience);
		if (level > mLevel)
		{
			addLevels(level);
		}
		if (mLevel < mMaxLevel || mMaxLevel <= 0)
		{
			mExperience = experience;
		}
	}
	int Levelable::getExperience() const
	{
		return mExperience;
	}

	void Levelable::addLevel(int level)
	{
		setLevel(mLevel + level);
	}
	void Levelable::setLevel(int level)
	{
		addLevels(level);
	}
	int Levelable::getLevel() const
	{
		return mLevel;
	}

	void Levelable::setMaxLevel(int level)
	{
		mMaxLevel = level;
	}
	int Levelable::getMaxLevel() const
	{
		return mMaxLevel;
	}

	int Levelable::getExperienceForLevel(int level) const
	{
		return level * 1000;
	}
	int Levelable::getLevelFromExperience(int experience) const
	{
		return experience / 1000;
	}

	void Levelable::addLevels(int level)
	{
		while (level > mLevel && (mMaxLevel == 0 || mLevel < mMaxLevel))
		{
			// Leveled up.
			mLevel++;
			mExperience = getExperienceForLevel(mLevel);
			onLevelUp();
		}
	}

}
}
