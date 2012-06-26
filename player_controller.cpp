#include "player_controller.h"

#include "keyboard_manager.h"
#include "keyboard_event.h"

#include "logger.h"

#include "character.h"

namespace am {
namespace base {

	PlayerController::PlayerController() :
		mMoveX(0),
		mMoveY(0),
		mRunning(false)
	{
		KeyboardManager::getManager()->addEventListener("key_down", this);
		KeyboardManager::getManager()->addEventListener("key_up", this);
	}
	PlayerController::~PlayerController()
	{
		KeyboardManager::getManager()->removeEventListener("key_down", this);
		KeyboardManager::getManager()->removeEventListener("key_up", this);
	}

	void PlayerController::onEvent(KeyboardEvent *e)
	{
			const bool *keys = KeyboardManager::getManager()->getKeysDown();
			int x = 0;
			if (keys[37])
			{
				x -= 1;
			}
			if (keys[39])
			{
				x += 1;
			}
			int y = 0;
			if (keys[38])
			{
				y -= 1;
			}
			if (keys[40])
			{
				y += 1;
			}
			mMoveX = x;
			mMoveY = y;
		
	}

	void PlayerController::update(Character *character, float dt)
	{
		character->setMoveVector(mMoveX, mMoveY);

		mMoveX = 0;
		mMoveY = 0;
	}

}
}
