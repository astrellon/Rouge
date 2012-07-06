#pragma once

namespace am {
namespace game {

	class Stat {
	public:
		enum StatType {
			HEALTH, 
			MAX_HEALTH, 
			STRENGTH, 
			DEXDERITY,
			CONSTITUTION,
			ARCANCE, 
			DIVINE,
			MIN_DAMAGE,
			MAX_DAMAGE,
			LUCK,
			MAX_STAT_LENGTH
		};
	};

}
}
