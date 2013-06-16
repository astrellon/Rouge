#include "ai_controller.h"

#include <log/logger.h>

#include <util/utils.h>
using namespace am::util;

#include "character.h"
#include "engine.h"
#include "game.h"

namespace am {
namespace game {

	AiController::AiController() :
		IController()
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

		float r = Utils::randf();
		int dx = (r > 0.9f ? 1 : (r < 0.1f ? -1 : 0));
		r = Utils::randf();
		int dy = (r > 0.9f ? 1 : (r < 0.1f ? -1 : 0));

		character->moveGrid(dx, dy);

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
