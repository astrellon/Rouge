#include "stat_event.h"

#include <game/stats.h>
using namespace am::game;

namespace am {
namespace ui {

	StatEvent::StatEvent(Stats *stats, Stat::StatType stat) :
		Event("stat_change"),
		mStats(stats),
		mStat(stat)
	{
		//setEventTarget(stats->getAttachedTo());
	}
	StatEvent::~StatEvent()
	{
	}

	Stats *StatEvent::getStats() const
	{
		return mStats;
	}
	Stat::StatType StatEvent::getStat() const
	{
		return mStat;
	}

}
}
