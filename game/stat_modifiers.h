#pragma once

#include <base/imanaged.h>

#include <map>

#include "stats_common.h"
#include "stat_modifier.h"
#include "istat_modifiers.h"

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;

	class StatModifiers : public IStatModifiers, public base::IManaged {
	public:

		StatModifiers();
		~StatModifiers();

		virtual bool addStatModifier(Stat::StatType stat, const StatModifier &modifier);
		virtual bool removeStatModifier(Stat::StatType stat, const StatModifier &modifier);

		virtual void addModifiers(const IStatModifiers &rhs);
		virtual void removeModifiers(const IStatModifiers &rhs);
		virtual void addModifiers(const IStatModifiers *rhs);
		virtual void removeModifiers(const IStatModifiers *rhs);

		virtual float calculateStat(Stat::StatType stat, float baseValue);

		virtual const StatModifierMap &getModifiers() const;
		virtual StatModifierMap &getModifiers();

		virtual data::IData *serialise();
		virtual void deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		StatModifierMap mModifiers;

		StatModifierVector::const_iterator findStatModifier(
			const StatModifierVector &modifiers, const StatModifier &modifier);
	};

}
}
