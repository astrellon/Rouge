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
			addLevelsUntil(level);
		}
		if (mLevel < mMaxLevel || mMaxLevel <= 0)
		{
			_setExperience(experience);
		}
	}
	int Levelable::getExperience() const
	{
		return mExperience;
	}

	void Levelable::addLevel(short level)
	{
		setLevel(mLevel + level);
	}
	void Levelable::setLevel(short level)
	{
		addLevelsUntil(level);
	}
	short Levelable::getLevel() const
	{
		return mLevel;
	}

	void Levelable::setMaxLevel(short level)
	{
		mMaxLevel = level;
	}
	short Levelable::getMaxLevel() const
	{
		return mMaxLevel;
	}

	int Levelable::getExperienceForLevel(short level) const
	{
		return level * 1000;
	}
	short Levelable::getLevelFromExperience(int experience) const
	{
		return experience / 1000;
	}

	void Levelable::addLevelsUntil(short level)
	{
		while (level > mLevel && (mMaxLevel == 0 || mLevel < mMaxLevel))
		{
			// Leveled up.
			mLevel++;
			mExperience = getExperienceForLevel(mLevel);
			onLevelUp();
		}
	}

	inline void Levelable::_setExperience(int exp)
	{
		if (mExperience != exp)
		{
			mExperience = exp;
			onExperienceChange();
		}
	}
}
}
