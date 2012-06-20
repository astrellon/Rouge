#include "player_controller.h"

#include "keyboard_manager.h"
#include "keyboard_event.h"

#include "logger.h"

namespace am {
namespace base {

	PlayerController::PlayerController()
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
		if (e->getKeyboardEventType() == KEY_DOWN)
		{
			am_log("KEY", "KEY DOWN");
		}
		else
		{
			am_log("KEY", "KEY UP");
		}
	}

	void PlayerController::update(Character *character, float dt)
	{

	}

}
}
