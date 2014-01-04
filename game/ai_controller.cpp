#include "ai_controller.h"

#include <log/logger.h>

#include <util/utils.h>

#include "character.h"
#include "engine.h"
#include "game.h"
#include "move_action.h"

namespace am {
namespace game {

	AiController::AiController() :
		IController(),
		mCharacter(nullptr)
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
		
	}
	void AiController::onGameTick(Character *character, float dt)
	{
		if (!isEnabled() || character->isDead())
		{
			return;
		}
		mActive = true;

		if (character->getAIFunc() != LUA_REFNIL)
		{
			// Call lua function.
			lua::LuaState &lua = Engine::getEngine()->getLua();
			lua_rawgeti(lua, LUA_REGISTRYINDEX, character->getAIFunc());
			lua::wrapRefObject<Character>(lua, character);
			lua_pushnumber(lua, dt);
			lua_acall(lua, 2, 0);
		}
		else
		{
			/*if (!character->hasDestination())
			{
				Map *map = Engine::getGame()->getCurrentMap();
				float destX = Utils::randf() * static_cast<float>(map->getMapWidth()) * Engine::gridSize();
				float destY = Utils::randf() * static_cast<float>(map->getMapHeight()) * Engine::gridSize();
				character->setDestination(destX, destY);
			}*/
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
				mCharacter = nullptr;
			}
		}
	}

}
}
