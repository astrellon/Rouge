#include "os_system.h"

#include <sys/game_system.h>

namespace am {
namespace sys {

	OsSystem::OsSystem() :
		mGameSystem(nullptr)
	{

	}
	OsSystem::~OsSystem()
	{
		
	}

	void OsSystem::setGameSystem(GameSystem *game)
	{
		mGameSystem = game;

	}
	GameSystem *OsSystem::getGameSystem()
	{
		return mGameSystem;
	}
}
}
