#pragma once

#include <string>
using namespace std;

#include "event.h"

#include <game/stats_common.h>

namespace am {
namespace game {
	class Stats;
}
using namespace am::game;

namespace ui {

	class StatEvent : public Event {
	public:
		StatEvent(Stats *stats, Stat::StatType stat);
		~StatEvent();

		Stats *getStats() const;
		Stat::StatType getStat() const;

	protected:

		Stats *mStats;
		Stat::StatType mStat;

	};

}
}
