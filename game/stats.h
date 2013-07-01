#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <ui/event_interface.h>
using namespace am::ui;

#include "stats_common.h"
#include "stat_modifier.h"
#include "stat_modifiers.h"
#include "istat_modifiers.h"

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class GameObject;
	class LoadingState;

	class Stats : public IStatModifiers, public IManaged {
	public:
		Stats();
		Stats(const Stats &copy);
		~Stats();

		virtual void setAttachedTo(GameObject *attached);
		virtual GameObject *getAttachedTo();

		virtual float getBaseStat(Stat::StatType stat) const;
		virtual void setBaseStat(Stat::StatType stat, float value);

		virtual float getStat(Stat::StatType stat);

		virtual bool addStatModifier(Stat::StatType stat, const StatModifier &modifier);
		virtual bool removeStatModifier(Stat::StatType stat, const StatModifier &modifier);

		virtual void addModifiers(const IStatModifiers &rhs);
		virtual void removeModifiers(const IStatModifiers &rhs);

		virtual float calculateStat(Stat::StatType type, float baseValue);

		virtual const StatModifierMap &getModifiers() const;
		virtual StatModifierMap &getModifiers();

		virtual StatModifiers *getStatModifiers();
		virtual const StatModifiers *getStatModifiers() const;

		virtual data::IData *serialise();
		virtual void deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		GameObject *mAttachedTo;

		float mBaseStats[Stat::MAX_STAT_LENGTH];
		float mCalculatedStats[Stat::MAX_STAT_LENGTH];

		bool mDirtyStats[Stat::MAX_STAT_LENGTH];

		virtual void setStatDirty(int stat);
		virtual void setStatDirty(Stat::StatType stat);
		
		Handle<StatModifiers> mModifiers;
		//StatModifiers mModifiers;
	};

}
}
