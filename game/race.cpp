#include "race.h"

namespace am {
namespace game {

	//Race::RaceMap Race::sRaces;
	//Race *Race::sUnknown = new Race("Unknown");
	const int Race::LUA_ID = __COUNTER__;
	const char *Race::LUA_TABLENAME = "am_game_Race";

	Race::Race(const char *raceName) :
		mRaceName(raceName)
	{

	}
	Race::~Race()
	{

	}

	const char *Race::getRaceName() const
	{
		return mRaceName.c_str();
	}

}
}
