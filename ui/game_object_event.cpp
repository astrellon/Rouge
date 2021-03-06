#include "game_object_event.h"

#include <game/game_object.h>

#include <ui/event_interface.h>

namespace am {
namespace ui {

	EventInterface *GameObjectEvent::sManager = nullptr;

	GameObjectEvent::GameObjectEvent(const char *type, game::GameObject *obj, ui::Mouse::Button button) :
		Event(type),
		mGameObject(obj),
		mMouseButton(button)
	{
		if (obj)
		{
			obj->retain();
		}
	}
	GameObjectEvent::~GameObjectEvent()
	{
		if (mGameObject)
		{
			mGameObject->release();
		}
	}

	game::GameObject *GameObjectEvent::getGameObject() const
	{
		return mGameObject;
	}
	ui::Mouse::Button GameObjectEvent::getMouseButton() const
	{
		return mMouseButton;
	}

	void GameObjectEvent::setManager(EventInterface *manager)
	{
		if (sManager)
		{
			sManager->release();
		}
		sManager = manager;
		if (sManager)
		{
			sManager->retain();
		}
	}
	EventInterface *GameObjectEvent::getManager()
	{
		return sManager;
	}

}
}
