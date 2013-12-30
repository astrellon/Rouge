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
		ui::KeyboardManager::getManager()->addEventListener(ui::Keyboard::KEY_DOWN, this);
		ui::KeyboardManager::getManager()->addEventListener(ui::Keyboard::KEY_UP, this);
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

	void PlayerController::onEvent(ui::KeyboardEvent *e)
	{
        Game *game = Engine::getGame();
        if (game->getUIKeyboardFocus())
        {
            return;
        }
		const bool *keys = ui::KeyboardManager::getManager()->getKeysDown();

		if (mActive)
		{
			int x = keys[ui::Keyboard::KEY_RIGHT_ARROW] - keys[ui::Keyboard::KEY_LEFT_ARROW];
			int y = keys[ui::Keyboard::KEY_DOWN_ARROW] - keys[ui::Keyboard::KEY_UP_ARROW];
			
			if (x != 0 || y != 0)
			{
				// Player has moved.
				move(x, y);
				return;
			}
			if (keys[ui::Keyboard::KEY_SPACE])
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

		//const bool *pressed = KeyboardManager::getManager()->getKeysPressed();
		/*if (pressed[40])
		{
			move(0, -1);
		}*/
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

		std::stringstream ss;
		ss << "Wait time: " << waitTime << " (" << mCharacter->getSpeed() << ")";
		//am_log("PLYR", ss);

		mCharacter->moveGrid(dx, dy);
		Game *game = Engine::getGame();
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
			ui::KeyboardManager::getManager()->removeEventListener(ui::Keyboard::KEY_DOWN, this);
			ui::KeyboardManager::getManager()->removeEventListener(ui::Keyboard::KEY_UP, this);
		}
	}

}
}
