#pragma once

#include "event.h"
#include "mouse_common.h"

namespace am {
namespace game {
	class GameObject;
}
using namespace am::game;

namespace ui {

	class EventInterface;

	class GameObjectEvent : public Event {
	public:
		GameObjectEvent(const char *type, GameObject *obj, Mouse::Button button);
		~GameObjectEvent();

		GameObject *getGameObject() const;
		Mouse::Button getMouseButton() const;

		static void setManager(EventInterface *manager);
		static EventInterface *getManager();

	protected:

		GameObject *mGameObject;
		Mouse::Button mMouseButton;

		static EventInterface *sManager;
	};

}
}
