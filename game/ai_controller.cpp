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

		/*float r = Utils::randf();
		int dx = (r > 0.9f ? 1 : (r < 0.1f ? -1 : 0));
		r = Utils::randf();
		int dy = (r > 0.9f ? 1 : (r < 0.1f ? -1 : 0));

		character->moveGrid(dx, dy);*/
		//character->move(5.0f, 0.0f);

		const Character::ActionQueue &queue = character->getActionQueue();
		if (queue.empty())
		{
			Map *map = Engine::getGame()->getCurrentMap();
			float destX = Utils::randf() * static_cast<float>(map->getMapWidth()) * Engine::getEngine()->getGridXSize();
			float destY = Utils::randf() * static_cast<float>(map->getMapHeight()) * Engine::getEngine()->getGridYSize();
			/*while (!map->isValidLocation(destX, destY, character))
			{
				destX = Utils::randf() * map->getMapWidth() * Engine::getEngine()->getGridXSize();
				destY = Utils::randf() * map->getMapHeight() * Engine::getEngine()->getGridYSize();
			}*/
			character->addAction(new MoveAction(destX, destY));
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
