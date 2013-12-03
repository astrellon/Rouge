#include "stat_event.h"

#include <game/stats.h>

namespace am {
namespace ui {

	StatEvent::StatEvent(game::Stats *stats, game::Stat::StatType stat) :
		Event("stat_change"),
		mStats(stats),
		mStat(stat)
	{
		//setEventTarget(stats->getAttachedTo());
	}
	StatEvent::~StatEvent()
	{
	}

	game::Stats *StatEvent::getStats() const
	{
		return mStats;
	}
	game::Stat::StatType StatEvent::getStat() const
	{
		return mStat;
	}

}
}
