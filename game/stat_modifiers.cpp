#include "stat_modifiers.h"

namespace am {
namespace game {

	const int StatModifiers::LUA_ID = __COUNTER__;
	const char *StatModifiers::LUA_TABLENAME = "am_game_StatModifiers";

	StatModifiers::StatModifiers()
	{
	}
	StatModifiers::~StatModifiers()
	{
	}

	bool StatModifiers::addStatModifier(Stat::StatType stat, const StatModifier &modifier)
	{
		if (stat >= Stat::MAX_STAT_LENGTH) 
		{
			return false;
		}

		StatModifierMap::iterator findModifiers = mModifiers.find(stat);
		if (findModifiers == mModifiers.end())
		{
			mModifiers[stat] = StatModifierVector();
		}
		StatModifierVector &modifiers = mModifiers[stat];
		StatModifierVector::const_iterator iter = findStatModifier(modifiers, modifier);
		if (iter == modifiers.end())
		{
			modifiers.push_back(modifier);
			return true;
		}
		return false;
	}
	bool StatModifiers::removeStatModifier(Stat::StatType stat, const StatModifier &modifier)
	{
		if (stat >= Stat::MAX_STAT_LENGTH)
		{
			return false;
		}
		StatModifierMap::iterator findModifiers = mModifiers.find(stat);
		if (findModifiers == mModifiers.end())
		{
			return false;
		}
		StatModifierVector &modifiers = findModifiers->second;
		StatModifierVector::const_iterator iter = findStatModifier(modifiers, modifier);
		if (iter != modifiers.end())
		{
			modifiers.erase(iter);
			return true;
		}
		return false;
	}

	StatModifiers::StatModifierVector::const_iterator StatModifiers::findStatModifier(
		const StatModifierVector &modifiers, const StatModifier &modifier)
	{
		StatModifiers::StatModifierVector::const_iterator iter;
		for (iter = modifiers.begin(); iter != modifiers.end(); ++iter)
		{
			if (*iter == modifier)
			{
				break;
			}
		}
		return iter;
	}

	void StatModifiers::addModifiers(const StatModifiers &rhs)
	{
		StatModifierMap::const_iterator rhsIter;
		for (rhsIter = rhs.mModifiers.begin(); rhsIter != rhs.mModifiers.end(); ++rhsIter)
		{
			const StatModifierVector &rhsModifiers = rhsIter->second;
			Stat::StatType stat = static_cast<Stat::StatType>(rhsIter->first);

			StatModifierVector::const_iterator iter;
			for (iter = rhsModifiers.begin(); iter != rhsModifiers.end(); ++iter)
			{
				addStatModifier(stat, *iter);
			}
		}
	}

	void StatModifiers::removeModifiers(const StatModifiers &rhs)
	{
		StatModifierMap::const_iterator rhsIter;
		for (rhsIter = rhs.mModifiers.begin(); rhsIter != rhs.mModifiers.end(); ++rhsIter)
		{
			const StatModifierVector &rhsModifiers = rhsIter->second;
			Stat::StatType stat = static_cast<Stat::StatType>(rhsIter->first);

			StatModifierVector::const_iterator iter;
			for (iter = rhsModifiers.begin(); iter != rhsModifiers.end(); ++iter)
			{
				removeStatModifier(stat, *iter);
			}
		}
	}

	float StatModifiers::calculateStat(Stat::StatType stat, float baseValue)
	{
		float value = baseValue;

		StatModifierType phase = MOD_SET;
		const StatModifierVector &modifiers = mModifiers[stat];
		int len = static_cast<int>(modifiers.size());
		while (true)
		{
			for (int i = 0; i < len; i++)
			{
				const StatModifier &modifier = modifiers[i];
				StatModifierType type = modifier.getType();
				if (type == phase)
				{
					if (phase == MOD_SET)
					{
						value = modifier.getValue();
					}
					else if (type == MOD_MULTIPLY)
					{
						value *= modifier.getValue();
					}
					else if (type == MOD_ADD)
					{
						value += modifier.getValue();
					}
				}
			}

			if (phase == MOD_SET)
			{
				phase = MOD_MULTIPLY;
			}
			else if (phase == MOD_MULTIPLY)
			{
				phase = MOD_ADD;
			}
			else
			{
				break;
			}
		}

		return value;
	}

	const StatModifiers::StatModifierMap &StatModifiers::getModifiers() const
	{
		return mModifiers;
	}

}
}
