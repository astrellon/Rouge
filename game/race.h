#pragma once

#include <string>
#include <map>
using namespace std;

namespace am {
namespace game {

	class Race {
	public:

		typedef map<string, Race *> RaceMap;

		Race(const char *raceName);
		~Race();

		const char *getRaceName() const;

		static bool addRace(Race *race);
		static bool removeRace(Race *race);
		static Race *getRace(const char *raceName);
		static Race *getUnknownRace();

	protected:

		string mRaceName;
		static RaceMap sRaces;
		static Race *sUnknown;
	};

}
}
