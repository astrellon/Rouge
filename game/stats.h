#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "stats_common.h"
#include "stat_modifier.h"
#include "stat_modifiers.h"

namespace am {
namespace game {

	class GameObject;

	class Stats {
	public:
		Stats();
		~Stats();

		virtual void setAttachedTo(GameObject *attached);
		virtual GameObject *getAttachedTo();

		virtual float getBaseStat(Stat::StatType stat) const;
		virtual void setBaseStat(Stat::StatType stat, float value);

		virtual float getStat(Stat::StatType stat);

		virtual void addStatModifier(Stat::StatType stat, const StatModifier &modifier);
		virtual void removeStatModifier(Stat::StatType stat, const StatModifier &modifier);

		virtual void addModifiers(const StatModifiers &rhs);
		virtual void removeModifiers(const StatModifiers &rhs);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		GameObject *mAttachedTo;

		float mBaseStats[Stat::MAX_STAT_LENGTH];
		float mCalculatedStats[Stat::MAX_STAT_LENGTH];

		bool mDirtyStats[Stat::MAX_STAT_LENGTH];
		
		StatModifiers mModifiers;

		void calculateStat(Stat::StatType stat);
	};

}
}
