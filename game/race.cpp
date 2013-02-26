#include "race.h"

namespace am {
namespace game {

	const int Race::LUA_ID = 0x0E;
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
