#include "stat_modifiers.h"

#include <util/data_table.h>
#include <util/data_string.h>
#include <util/data_number.h>

#include <sstream>

#include <log/logger.h>

#include "loading_state.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int StatModifiers::LUA_ID = LUA_ID_STATMODIFIERS;
	const char *StatModifiers::LUA_TABLENAME = LUA_TABLE_STATMODIFIERS;

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
		auto i = findStatModifier(modifiers, modifier);
		if (i == -1u)
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
		auto i = findStatModifier(modifiers, modifier);
		if (i != -1u)
		{
			modifiers.erase(modifiers.begin() + i);
			return true;
		}
		return false;
	}

	size_t StatModifiers::findStatModifier(
		const StatModifierVector &modifiers, const StatModifier &modifier)
	{
        for (size_t i = 0; i < modifiers.size(); i++)
		{
			if (modifiers[i] == modifier)
			{
				return i;
			}
		}
		return -1u;
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
	void StatModifiers::addModifiers(const IStatModifiers *rhs)
	{
		if (rhs)
		{
			addModifiers(*rhs);
		}
	}
	void StatModifiers::removeModifiers(const IStatModifiers *rhs)
	{
		if (rhs)
		{
			removeModifiers(*rhs);
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
		data::Table *output = new data::Table();
		for (auto iter = mModifiers.begin(); iter != mModifiers.end(); ++iter)
		{
			data::Table *modifiers = new data::Table();
			for (auto modIter = iter->second.begin(); modIter != iter->second.end(); ++modIter)
			{
				modifiers->push(modIter->serialise());
			}
			output->at(Stat::getStatName(iter->first), modifiers);
		}

		return output;
	}
	void StatModifiers::deserialise(LoadingState *state, data::IData *data)
	{
		base::Handle<data::Table> dataMap(data::Table::checkDataType(data, "stat modifiers"));
		if (!dataMap)
		{
			return;
		}
		
		for (auto iter = dataMap->beginMap(); iter != dataMap->endMap(); ++iter)
		{
			Stat::StatType type = Stat::getStatType(iter->first.c_str());
			if (type == Stat::MAX_STAT_LENGTH)
			{
				std::stringstream ss;
				ss << "Unknown stat type '" << iter->first << "', unable to load stat modifiers.";
				am_log("LOADERR", ss);
				continue;
			}

			base::Handle<data::Table> arr(data::Table::checkDataType(iter->second.get(), "stat modifiers"));
			if (arr)
			{
				continue;
			}

			for (auto modIter = arr->beginArray(); modIter != arr->endArray(); ++iter)
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
