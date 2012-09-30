#include "stat_modifier.h"

#include <util/utils.h>
using namespace am::util;

#include <string>
#include <cstring>
#include <time.h>

#include "stats.h"

namespace am {
namespace game {

	const char *StatModifier::sStatModifierNames[] = {
		"+", "*", "*=", "MOD_MAX_LENGTH"
	};

	StatModifier::StatModifier() :
		mValue(0),
		mType(MOD_ADD),
		mStatsParent(NULL),
		mMagical(true)
	{

	}
	StatModifier::StatModifier(float value, StatModifierType type, bool magical) :
		mValue(value),
		mType(type),
		mStatsParent(NULL),
		mMagical(magical)
	{

	}
	StatModifier::~StatModifier()
	{
	}

	void StatModifier::setStatsParent(Stats *parent)
	{
		mStatsParent = parent;
	}
	Stats *StatModifier::getStatsParent()
	{
		return mStatsParent;
	}
		
	void StatModifier::setValue(float value)
	{
		mValue = value;
	}
	float StatModifier::getValue() const
	{
		return mValue;
	}

	void StatModifier::setMagical(bool magical)
	{
		mMagical = magical;
	}
	bool StatModifier::isMagical() const
	{
		return mMagical;
	}

	void StatModifier::setType(StatModifierType type)
	{
		mType = type;
	}
	StatModifierType StatModifier::getType() const
	{
		return mType;
	}

	bool StatModifier::operator==(const StatModifier &rhs) const
	{
		return rhs.mValue == mValue && rhs.mType == mType && rhs.mStatsParent == mStatsParent;
	}
	bool StatModifier::operator!=(const StatModifier &rhs) const
	{
		return rhs.mValue != mValue || rhs.mType != mType || rhs.mStatsParent != mStatsParent;
	}

	StatModifierType StatModifier::getModifierType(const char *name)
	{
		for (int i = 0; i < MOD_MAX_LENGTH; i++)
		{
			if (strcmp(name, sStatModifierNames[i]) == 0)
			{
				return static_cast<StatModifierType>(i);
			}
		}
		return MOD_MAX_LENGTH;
	}
	StatModifierType StatModifier::getModifierType(int value)
	{
		if (value < 0 || value >= MOD_MAX_LENGTH)
		{
			return MOD_MAX_LENGTH;
		}
		return static_cast<StatModifierType>(value);
	}
	
}
}
