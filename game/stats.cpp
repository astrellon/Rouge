#include "stats.h"

#include "game_object.h"

namespace am {
namespace game {

	const int Stats::LUA_ID = 0x02;
	const char *Stats::LUA_TABLENAME = "am_game_Stats";

	Stats::Stats()
	{
		for (int i = 0; i < Stat::MAX_STAT_LENGTH; i++)
		{
			mBaseStats[i] = 0.0f;
			mCalculatedStats[i] = 0.0f;
			mDirtyStats[i] = true;
		}
	}
	Stats::~Stats()
	{

	}

	float Stats::getStat(Stat::StatType stat)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return 0.0f;
		}
		if (mDirtyStats[stat])
		{
			calculateStat(stat);
		}
		return mCalculatedStats[stat];
	}

	void Stats::addStatModifier(Stat::StatType stat, const StatModifier &modifier)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return;
		}
		if (mModifiers.addStatModifier(stat, modifier))
		{
			mDirtyStats[stat] = true;
		}
	}
	void Stats::removeStatModifier(Stat::StatType stat, const StatModifier &modifier)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return;
		}
		if (mModifiers.removeStatModifier(stat, modifier))
		{
			mDirtyStats[stat] = true;
		}
	}

	float Stats::getBaseStat(Stat::StatType stat) const
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return 0.0f;
		}
		return mBaseStats[stat];
	}
	void Stats::setBaseStat(Stat::StatType stat, float value)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return;
		}
		if (mBaseStats[stat] != value)
		{
			mBaseStats[stat] = value;
			mDirtyStats[stat] = true;
		}
	}

	void Stats::calculateStat(Stat::StatType stat)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return;
		}

		float value = mBaseStats[stat];
		value = mModifiers.calculateStat(stat, value);
		mDirtyStats[stat] = false;
		mCalculatedStats[stat] = value;
	}

	void Stats::setAttachedTo(GameObject *attached)
	{
		if (mAttachedTo)
		{
			mAttachedTo->release();
		}
		mAttachedTo = attached;
		if (mAttachedTo)
		{
			mAttachedTo->retain();
		}
	}
	GameObject *Stats::getAttachedTo()
	{
		return mAttachedTo;
	}
	
	void Stats::addModifiers(const StatModifiers &rhs)
	{
		mModifiers.addModifiers(rhs);
		const StatModifiers::StatModifierMap &modifiers = rhs.getModifiers();
		StatModifiers::StatModifierMap::const_iterator iter;
		for (iter = modifiers.begin(); iter != modifiers.end(); ++iter)
		{
			mDirtyStats[iter->first] = true;
		}
	}
	void Stats::removeModifiers(const StatModifiers &rhs)
	{
		mModifiers.removeModifiers(rhs);
		const StatModifiers::StatModifierMap &modifiers = rhs.getModifiers();
		StatModifiers::StatModifierMap::const_iterator iter;
		for (iter = modifiers.begin(); iter != modifiers.end(); ++iter)
		{
			mDirtyStats[iter->first] = true;
		}
	}

	StatModifiers &Stats::getModifiers()
	{
		return mModifiers;
	}
	const StatModifiers &Stats::getModifiers() const
	{
		return mModifiers;
	}
	
}
}
