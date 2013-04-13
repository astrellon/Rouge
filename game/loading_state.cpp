#include "loading_state.h"

namespace am {
namespace game {

	LoadingState::LoadingState()
	{

	}
	LoadingState::~LoadingState()
	{

	}

	void LoadingState::addMapToLoad(const char *mapName)
	{
		mMapsToLoad[string(mapName)] = true;
	}
	const LoadingState::MapsToLoad &LoadingState::getMapsToLoad() const
	{
		return mMapsToLoad;
	}

	void LoadingState::setCurrentMap(const char *mapName)
	{
		mCurrentMap = mapName;
		addMapToLoad(mapName);
	}
	const char *LoadingState::getCurrentMap() const
	{
		return mCurrentMap.c_str();
	}

	void LoadingState::setMainCharacter(const char *mainChar)
	{
		mMainCharacter = mainChar;
	}
	const char *LoadingState::getMainCharacter() const
	{
		return mMainCharacter.c_str();
	}

}
}
