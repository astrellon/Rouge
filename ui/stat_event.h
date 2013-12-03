#pragma once

#include <string>

#include "event.h"

#include <game/stats_common.h>

namespace am {
namespace game {
	class Stats;
}

namespace ui {

	class StatEvent : public Event
	{
	public:
		StatEvent(game::Stats *stats, game::Stat::StatType stat);
		~StatEvent();

		game::Stats *getStats() const;
		game::Stat::StatType getStat() const;

	protected:

		game::Stats *mStats;
		game::Stat::StatType mStat;

	};

}
}
