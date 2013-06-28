#pragma once

namespace am {
namespace game {

	class Stat {
	public:
		enum StatType {
			HEALTH, 
			MAX_HEALTH, 
			STRENGTH, 
			DEXTERITY,
			CONSTITUTION,
			ARCANE, 
			DIVINE,
			MIN_DAMAGE,
			MAX_DAMAGE,
			ARMOUR,
			SPEED,
			MAX_STAT_LENGTH
		};

		static const char *getNiceStatName(StatType stat);
		static const char *getStatName(StatType stat);
		static const char *getStatName(int stat);

		static StatType getStatType(int statValue);
		static StatType getStatTypeFromNice(const char *name);
		static StatType getStatType(const char *name);

	protected:
		static const char *sNiceStatNames[];
		static const char *sStatNames[];

	};

}
}
