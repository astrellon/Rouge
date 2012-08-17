#include "stats_common.h"

#include <cstring>

namespace am {
namespace game {

	const char *Stat::sNiceStatNames[] = {
		"Health", "Max Health", "Strength", "Dexterity", "Constitution",
		"Arcance", "Divine", "Min Damage", "Max Damage", "MAX_STAT_LENGTH", "UNKNOWN"
	};
	const char *Stat::sStatNames[] = {
		"health", "maxHealth", "strength", "dexterity", "constitution",
		"arcance", "divine", "minDamage", "maxDamage", "MAX_STAT_LENGTH", "UNKNOWN"
	};

	const char *Stat::getNiceStatName(StatType stat)
	{
		return sNiceStatNames[stat];
	}
	const char *Stat::getStatName(StatType stat)
	{
		return sStatNames[stat];
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
