#include "race.h"

namespace am {
namespace game {

	Race::RaceMap Race::sRaces;
	Race *Race::sUnknown = new Race("Unknown");

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

	bool Race::addRace(Race *race)
	{
		if (race == NULL)
		{
			return false;
		}
		RaceMap::const_iterator iter = sRaces.find(race->mRaceName);
		if (iter == sRaces.end())
		{
			sRaces[race->mRaceName] = race;
			return true;
		}
		return false;
	}
	bool Race::removeRace(Race *race)
	{
		if (race == NULL)
		{
			return false;
		}
		RaceMap::const_iterator iter = sRaces.find(race->mRaceName);
		if (iter != sRaces.end())
		{
			sRaces.erase(iter);
			return true;
		}
		return false;
	}
	Race *Race::getRace(const char *raceName)
	{
		if (raceName == NULL || raceName[0] == '\0')
		{
			return NULL;
		}
		RaceMap::iterator iter = sRaces.find(string(raceName));
		if (iter == sRaces.end())
		{
			return NULL;
		}
		return iter->second;
	}
	Race *Race::getUnknownRace()
	{
		return sUnknown;
	}

}
}
