#include "ai_controller.h"

#include <log/logger.h>

#include <util/utils.h>
using namespace am::util;

#include "character.h"
#include "engine.h"
#include "game.h"
#include "move_action.h"

namespace am {
namespace game {

	AiController::AiController() :
		IController(),
		mCharacter(NULL)
	{
		mAttached = true;
		mRemoved = false;
	}
	AiController::~AiController()
	{
		if (mRemoved)
		{
			return;
		}
		mRemoved = true;
		detach();
	}

	void AiController::update(Character *character, float dt)
	{
		mActive = true;

		if (!character->hasDestination())
		{
			Map *map = Engine::getGame()->getCurrentMap();
			float destX = Utils::randf() * static_cast<float>(map->getMapWidth()) * Engine::gridSize();
			float destY = Utils::randf() * static_cast<float>(map->getMapHeight()) * Engine::gridSize();
			character->setDestination(destX, destY);
		}

		mActive = false;
	}

	void AiController::detach()
	{
		if (mAttached)
		{
			mAttached = false;
			mActive = false;
			if (mCharacter)
			{
				mCharacter->release();
				mCharacter = NULL;
			}
		}
	}

}
}
