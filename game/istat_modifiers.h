#pragma once

#include <map>

#include "stats_common.h"
#include "stat_modifier.h"

namespace am {
namespace game {

	class IStatModifiers 
	{
	public:

		typedef std::vector<StatModifier> StatModifierVector;
		typedef std::map<int, StatModifierVector> StatModifierMap;

		virtual bool addStatModifier(Stat::StatType stat, const StatModifier &modifier) = 0;
		virtual bool removeStatModifier(Stat::StatType stat, const StatModifier &modifier) = 0;

		virtual void addModifiers(const IStatModifiers &rhs) = 0;
		virtual void removeModifiers(const IStatModifiers &rhs) = 0;
		virtual void addModifiers(const IStatModifiers *rhs) = 0;
		virtual void removeModifiers(const IStatModifiers *rhs) = 0;

		virtual float calculateStat(Stat::StatType stat, float baseValue) = 0;

		virtual const StatModifierMap &getModifiers() const = 0;
		virtual StatModifierMap &getModifiers() = 0;

	};

}
}
