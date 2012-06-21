#include "player_controller.h"

#include "keyboard_manager.h"
#include "keyboard_event.h"

#include "logger.h"

#include "character.h"

namespace am {
namespace base {

	PlayerController::PlayerController() :
		mMoveX(0.0f),
		mMoveY(0.0f),
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
		float x = 0.0f;
		if (keys[37])
		{
			x -= 1.0f;
		}
		if (keys[39])
		{
			x += 1.0f;
		}
		float y = 0.0f;
		if (keys[38])
		{
			y -= 1.0f;
		}
		if (keys[40])
		{
			y += 1.0f;
		}
		if (x != 0.0f || y != 0.0f)
		{
			float length = x * x + y * y;
			if (length != 1.0f)
			{
				length = 1.0f / sqrt(length);
				x *= length;
				y *= length;
			}
		}
		mMoveX = x;
		mMoveY = y;
		
		if (e->getKeyboardEventType() == KEY_UP)
		{
			// Caps Locks
			if (e->getKey() == 20)
			{
				mRunning = !mRunning;
			}
		}
	}

	void PlayerController::update(Character *character, float dt)
	{
		character->setMoveVector(mMoveX, mMoveY);
		character->setRunning(mRunning);
	}

}
}
