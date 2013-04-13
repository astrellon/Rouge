#pragma once

#include <base/imanaged.h>
using namespace am::base;

#include <string>
#include <map>
#include <vector>
using namespace std;

namespace am {
namespace game {

	class LoadingState : public IManaged {
	public:

		typedef map<string, bool> MapsToLoad;

		LoadingState();
		~LoadingState();

		void addMapToLoad(const char *map);
		const MapsToLoad &getMapsToLoad() const;

		void setCurrentMap(const char *mapName);
		const char *getCurrentMap() const;

		void setMainCharacter(const char *mainChar);
		const char *getMainCharacter() const;

	protected:

		MapsToLoad mMapsToLoad;

		string mCurrentMap;
		string mMainCharacter;
	};

}
}
