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
			ARCANCE, 
			DIVINE,
			MIN_DAMAGE,
			MAX_DAMAGE,
			MAX_STAT_LENGTH
		};

		static const char *getStatName(StatType stat);

	protected:
		static const char *sStatNames[];

	};

}
}
