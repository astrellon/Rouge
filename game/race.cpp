#include "race.h"

namespace am {
namespace game {

	//Race::RaceMap Race::sRaces;
	//Race *Race::sUnknown = new Race("Unknown");

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
