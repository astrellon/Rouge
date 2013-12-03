#pragma once

#include "event.h"
#include "mouse_common.h"

namespace am {
namespace game {
	class GameObject;
}

namespace ui {

	class EventInterface;

	class GameObjectEvent : public Event 
	{
	public:
		GameObjectEvent(const char *type, game::GameObject *obj, Mouse::Button button);
		~GameObjectEvent();

		game::GameObject *getGameObject() const;
		Mouse::Button getMouseButton() const;

		static void setManager(EventInterface *manager);
		static EventInterface *getManager();

	protected:

		game::GameObject *mGameObject;
		Mouse::Button mMouseButton;

		static EventInterface *sManager;
	};

}
}
