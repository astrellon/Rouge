#include "player_controller.h"

#include <ui/keyboard_manager.h>
#include <ui/keyboard_event.h>

#include <log/logger.h>

#include "engine.h"
#include "game.h"

namespace am {
namespace game {

	PlayerController::PlayerController() :
		IController(),
		mRunning(false)
	{
		KeyboardManager::getManager()->addEventListener("key_down", this);
		KeyboardManager::getManager()->addEventListener("key_up", this);
		mAttached = true;
		mRemoved = false;
	}
	PlayerController::~PlayerController()
	{
		if (mRemoved)
		{
			return;
		}
		mRemoved = true;
		detach();
	}

	void PlayerController::onEvent(KeyboardEvent *e)
	{
		const bool *keys = KeyboardManager::getManager()->getKeysDown();

		if (mActive)
		{
			int x = keys[39] - keys[37];
			int y = keys[40] - keys[38];
			if (x != 0 || y != 0)
			{
				// Player has moved.
				move(x, y);
				return;
			}
			if (keys[' '])
			{
				// Wait for 1 second.
				am_log("PLYER", "Wait");
				setGameTick(1.0f);
				return;
			}
		}
	}

	void PlayerController::update(Character *character, float dt)
	{
		mCharacter = character;
		Engine::getGame()->setGameTickPaused(true);
		mActive = true;

		const bool *pressed = KeyboardManager::getManager()->getKeysPressed();
		if (pressed[40])
		{
			move(0, -1);
		}
		/*if (mMoveX != 0 && mMoveY != 0)
		{
			performAction();
		}*/
	}

	void PlayerController::move(int dx, int dy)
	{
		float waitTime = 1.0f;
		if (dx != 0 || dy != 0)
		{
			float dist = sqrt(static_cast<float>(dx * dx + dy * dy));
			waitTime = dist / mCharacter->getSpeed();
		}

		stringstream ss;
		ss << "Wait time: " << waitTime << " (" << mCharacter->getSpeed() << ")";
		am_log("PLYER", ss);

		mCharacter->moveGrid(dx, dy);
		mCharacter = nullptr;

		mActive = false;

		setGameTick(waitTime);
	}

	void PlayerController::setGameTick(float dt)
	{
		Game *game = Engine::getGame();
		game->setCurrentGameTickLength(dt);
		game->setGameTickPaused(false);
	}

	void PlayerController::detach()
	{
		if (mAttached)
		{
			mAttached = false;
			mActive = false;
			if (mCharacter)
			{
				mCharacter->release();
				mCharacter = nullptr;
			}
			KeyboardManager::getManager()->removeEventListener("key_down", this);
			KeyboardManager::getManager()->removeEventListener("key_up", this);
		}
	}

}
}
