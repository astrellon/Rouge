#include "stat_modifier.h"

#include <util/stat_tokeniser.h>
#include <util/utils.h>
using namespace am::util;

#include <string>
#include <cstring>
#include <time.h>

#include "stats.h"

namespace am {
namespace game {

	StatModifier::StatModifier() :
		mValue(0),
		mType(MOD_ADD),
		mStatsParent(NULL)
	{

	}
	StatModifier::StatModifier(float value, MODIFIER_TYPE type) :
		mValue(value),
		mType(type),
		mStatsParent(NULL)
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

	void StatModifier::setType(MODIFIER_TYPE type)
	{
		mType = type;
	}
	MODIFIER_TYPE StatModifier::getType() const
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
	
}
}
