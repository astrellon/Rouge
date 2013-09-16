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
		GameObjectEvent(const char *type, GameObject *obj, MouseButton button);
		~GameObjectEvent();

		GameObject *getGameObject() const;
		MouseButton getMouseButton() const;

		static void setManager(EventInterface *manager);
		static EventInterface *getManager();

	protected:

		GameObject *mGameObject;
		MouseButton mMouseButton;

		static EventInterface *sManager;
	};

}
}
