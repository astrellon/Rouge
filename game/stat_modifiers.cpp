#include "stat_modifiers.h"

#include <util/data_map.h>
#include <util/data_array.h>
#include <util/data_string.h>
#include <util/data_number.h>

#include <sstream>

#include <log/logger.h>

#include "loading_state.h"

namespace am {
namespace game {

	const int StatModifiers::LUA_ID = 0x01;
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

	void StatModifiers::addModifiers(const IStatModifiers &rhs)
	{
		const StatModifierMap &mods = rhs.getModifiers();
		StatModifierMap::const_iterator rhsIter;
		for (rhsIter = mods.begin(); rhsIter != mods.end(); ++rhsIter)
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

	void StatModifiers::removeModifiers(const IStatModifiers &rhs)
	{
		const StatModifierMap &mods = rhs.getModifiers();
		StatModifierMap::const_iterator rhsIter;
		for (rhsIter = mods.begin(); rhsIter != mods.end(); ++rhsIter)
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

	const IStatModifiers::StatModifierMap &StatModifiers::getModifiers() const
	{
		return mModifiers;
	}
	IStatModifiers::StatModifierMap &StatModifiers::getModifiers()
	{
		return mModifiers;
	}

	data::IData *StatModifiers::serialise()
	{
		data::Map *output = new data::Map();
		for (auto iter = mModifiers.begin(); iter != mModifiers.end(); ++iter)
		{
			data::Array *modifiers = new data::Array();
			for (auto modIter = iter->second.begin(); modIter != iter->second.end(); ++modIter)
			{
				modifiers->push(modIter->serialise());
			}
			output->push(Stat::getStatName(iter->first), modifiers);
		}

		return output;
	}
	void StatModifiers::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Map> dataMap(data::Map::checkDataType(data, "stat modifiers"));
		if (!dataMap)
		{
			return;
		}
		
		for (auto iter = dataMap->begin(); iter != dataMap->end(); ++iter)
		{
			Stat::StatType type = Stat::getStatType(iter->first.c_str());
			if (type == Stat::MAX_STAT_LENGTH)
			{
				stringstream ss;
				ss << "Unknown stat type '" << iter->first << "', unable to load stat modifiers.";
				am_log("LOADERR", ss);
				continue;
			}

			Handle<data::Array> arr(data::Array::checkDataType(iter->second.get(), "stat modifiers"));
			if (arr)
			{
				continue;
			}

			for (auto modIter = arr->begin(); modIter != arr->end(); ++iter)
			{
				StatModifier mod;
				if (mod.deserialise(state, modIter->get()))
				{
					addStatModifier(type, mod);
				}
			}
		}
	}

}
}
