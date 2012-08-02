#include "stats_common.h"

namespace am {
namespace game {

	const char *Stat::sStatNames[] = {
		"Health", "Max Health", "Strength", "Dexterity", "Constitution",
		"Arcance", "Divine", "Min Damage", "Max Damage", "MAX_STAT_LENGTH", "UNKNOWN"
	};

	const char *Stat::getStatName(StatType stat)
	{
		return sStatNames[stat];
	}


}
}
