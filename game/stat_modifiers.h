#pragma once

#include <map>
using namespace std;

#include "stats_common.h"
#include "stat_modifier.h"
#include "i_stat_modifiers.h"

namespace am {
namespace game {

	class StatModifiers : public IStatModifiers {
	public:

		StatModifiers();
		~StatModifiers();

		virtual bool addStatModifier(Stat::StatType stat, const StatModifier &modifier);
		virtual bool removeStatModifier(Stat::StatType stat, const StatModifier &modifier);

		virtual void addModifiers(const IStatModifiers &rhs);
		virtual void removeModifiers(const IStatModifiers &rhs);

		virtual float calculateStat(Stat::StatType stat, float baseValue);

		virtual const StatModifierMap &getModifiers() const;
		virtual StatModifierMap &getModifiers();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		StatModifierMap mModifiers;


		StatModifierVector::const_iterator findStatModifier(
			const StatModifierVector &modifiers, const StatModifier &modifier);
	};

}
}
