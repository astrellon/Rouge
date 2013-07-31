#include "stats_common.h"

#include <cstring>

namespace am {
namespace game {

	const char *Stat::sNiceStatNames[] = {
		"Health", "Max Health", "Strength", "Dexterity", "Constitution",
		"Arcane", "Divine", "Min Damage", "Max Damage", "Armour", "Speed", "MAX_STAT_LENGTH", "UNKNOWN"
	};
	const char *Stat::sStatNames[] = {
		"health", "maxHealth", "strength", "dexterity", "constitution",
		"arcane", "divine", "minDamage", "maxDamage", "armour", "speed", "MAX_STAT_LENGTH", "UNKNOWN"
	};

	const char *Stat::getNiceStatName(StatType stat)
	{
		if (stat < 0 || stat >= Stat::MAX_STAT_LENGTH)
		{
			return nullptr;
		}
		return sNiceStatNames[stat];
	}
	const char *Stat::getStatName(StatType stat)
	{
		if (stat < 0 || stat >= Stat::MAX_STAT_LENGTH)
		{
			return nullptr;
		}
		return sStatNames[stat];
	}
	const char *Stat::getStatName(int stat)
	{
		if (stat < 0 || stat >= Stat::MAX_STAT_LENGTH)
		{
			return nullptr;
		}
		return sStatNames[stat];
	}

	Stat::StatType Stat::getStatType(int statValue)
	{
		if (statValue < 0 || statValue >= Stat::MAX_STAT_LENGTH)
		{
			return Stat::MAX_STAT_LENGTH;
		}
		return static_cast<Stat::StatType>(statValue);
	}
	Stat::StatType Stat::getStatTypeFromNice(const char *name)
	{
		int type = HEALTH;
		while (type < MAX_STAT_LENGTH)
		{
			if (strcmp(name, sNiceStatNames[type]) == 0)
			{
				return static_cast<Stat::StatType>(type);
			}
			type++;
		}
		return MAX_STAT_LENGTH;
	}
	Stat::StatType Stat::getStatType(const char *name)
	{
		int type = HEALTH;
		while (type < MAX_STAT_LENGTH)
		{
			if (strcmp(name, sStatNames[type]) == 0)
			{
				return static_cast<Stat::StatType>(type);
			}
			type++;
		}
		return MAX_STAT_LENGTH;
	}


}
}
