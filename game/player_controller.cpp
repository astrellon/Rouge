#include "player_controller.h"

#include <ui/keyboard_manager.h>
#include <ui/keyboard_event.h>

#include <log/logger.h>

#include "character.h"
#include "engine.h"
#include "game.h"

namespace am {
namespace game {

	PlayerController::PlayerController() :
		IController(),
		mMoveX(0),
		mMoveY(0),
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

		if (mActive)
		{
			performAction();
		}
	}

	void PlayerController::update(Character *character, float dt)
	{
		mCharacter = character;
		mCharacter->retain();
		Engine::getGame()->setGameTickPaused(true);
		mActive = true;
		if (mMoveX != 0 && mMoveY != 0)
		{
			performAction();
		}
	}

	void PlayerController::performAction()
	{
		mCharacter->moveGrid(mMoveX, mMoveY);
		mCharacter->release();
		mCharacter = NULL;

		mMoveX = 0;
		mMoveY = 0;

		mActive = false;

		Game *game = Engine::getGame();
		game->setCurrentGameTickLength(1.0f);
		game->setGameTickPaused(false);
		//game->nextObjectTurn();
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
				mCharacter = NULL;
			}
			KeyboardManager::getManager()->removeEventListener("key_down", this);
			KeyboardManager::getManager()->removeEventListener("key_up", this);
		}
	}

}
}
