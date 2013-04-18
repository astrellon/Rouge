#include "stats.h"

#include "game_object.h"

#include <util/data_map.h>

namespace am {
namespace game {

	const int Stats::LUA_ID = 0x02;
	const char *Stats::LUA_TABLENAME = "am_game_Stats";

	Stats::Stats() :
		mAttachedTo(NULL)
	{
		for (int i = 0; i < Stat::MAX_STAT_LENGTH; i++)
		{
			mBaseStats[i] = 0.0f;
			mCalculatedStats[i] = 0.0f;
			mDirtyStats[i] = true;
		}
	}
	Stats::Stats(const Stats &copy) :
		mAttachedTo(NULL)
	{
		for (int i = 0; i < Stat::MAX_STAT_LENGTH; i++)
		{
			mBaseStats[i] = copy.mBaseStats[i];
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
			calculateStat(stat, mBaseStats[stat]);
		}
		return mCalculatedStats[stat];
	}

	bool Stats::addStatModifier(Stat::StatType stat, const StatModifier &modifier)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return false;
		}
		if (mModifiers.addStatModifier(stat, modifier))
		{
			setStatDirty(stat);
			return true;
		}
		return false;
	}
	bool Stats::removeStatModifier(Stat::StatType stat, const StatModifier &modifier)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return false;
		}
		if (mModifiers.removeStatModifier(stat, modifier))
		{
			setStatDirty(stat);
			return true;
		}
		return false;
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
			setStatDirty(stat);
		}
	}

	float Stats::calculateStat(Stat::StatType stat, float baseValue)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return -1.0f;
		}

		float value = mModifiers.calculateStat(stat, baseValue);
		mDirtyStats[stat] = false;
		mCalculatedStats[stat] = value;
		return value;
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
	
	void Stats::addModifiers(const IStatModifiers &rhs)
	{
		mModifiers.addModifiers(rhs);
		const StatModifiers::StatModifierMap &modifiers = rhs.getModifiers();
		StatModifiers::StatModifierMap::const_iterator iter;
		for (iter = modifiers.begin(); iter != modifiers.end(); ++iter)
		{
			setStatDirty(iter->first);
		}
	}
	void Stats::removeModifiers(const IStatModifiers &rhs)
	{
		mModifiers.removeModifiers(rhs);
		const StatModifiers::StatModifierMap &modifiers = rhs.getModifiers();
		StatModifiers::StatModifierMap::const_iterator iter;
		for (iter = modifiers.begin(); iter != modifiers.end(); ++iter)
		{
			mDirtyStats[iter->first] = true;
		}
	}

	const IStatModifiers::StatModifierMap &Stats::getModifiers() const
	{
		return mModifiers.getModifiers();
	}
	IStatModifiers::StatModifierMap &Stats::getModifiers()
	{
		return mModifiers.getModifiers();
	}

	StatModifiers &Stats::getStatModifiers()
	{
		return mModifiers;
	}
	const StatModifiers &Stats::getStatModifiers() const
	{
		return mModifiers;
	}

	void Stats::setStatDirty(int stat)
	{
		mDirtyStats[stat] = true;
		if (mAttachedTo)
		{
			Handle<StatEvent> e(new StatEvent(this, Stat::getStatType(stat)));
			mAttachedTo->fireEvent<StatEvent>(e);
		}
	}
	void Stats::setStatDirty(Stat::StatType stat)
	{
		mDirtyStats[stat] = true;
		if (mAttachedTo)
		{
			Handle<StatEvent> e(new StatEvent(this, stat));
			mAttachedTo->fireEvent<StatEvent>(e);
		}
	}

	data::IData *Stats::serialise()
	{
		data::Map *output = new data::Map();
		data::Map *baseStats = new data::Map();
		for (int i = 0; i < Stat::MAX_STAT_LENGTH; i++)
		{
			baseStats->push(Stat::getStatName(i), mBaseStats[i]);
		}
		output->push("baseStats", baseStats);
		output->push("modifiers", mModifiers.serialise());

		return output;
	}
	
}
}
