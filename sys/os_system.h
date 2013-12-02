#pragma once

#include "isystem.h"

namespace am {
namespace sys {

	class GameSystem;

	class OsSystem : public ISystem
	{
	public:
		
		~OsSystem();

		virtual void setGameSystem(GameSystem *game);
		virtual GameSystem *getGameSystem();

	protected:

		OsSystem();

		GameSystem *mGameSystem;
	};

}
}
