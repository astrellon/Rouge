#pragma once

#include "event.h"
#include "ievent_listener.h"
#include "event_manager.h"

#include "mouse_common.h"
#include "keyboard_common.h"

#include <string>

using namespace std;

namespace am {
namespace ui {

	class EventInterface {
	public:

		EventInterface();
		virtual ~EventInterface();

		bool addEventListener(const char *type, IEventListener *context);
		bool addEventListener(const string &type, IEventListener *context);
		bool addEventListener(MouseEventType type, IEventListener *context);
		bool addEventListener(KeyboardEventType type, IEventListener *context);
		
		bool removeEventListener(const char *type, IEventListener *context);
		bool removeEventListener(const string &type, IEventListener *context);
		bool removeEventListener(MouseEventType type, IEventListener *context);
		bool removeEventListener(KeyboardEventType type, IEventListener *context);
		void removeEventListener(IEventListener *context);
		
		bool hasEventListener(const char *type);
		bool hasEventListener(const string &type);

		template <class T>
		void fireEvent(T *e)
		{
			if (mManager == NULL)
			{
				return;
			}
			if (e->getEventTarget() == NULL)
			{
				e->setEventTarget(this);
			}
			mManager->fireEvent<T>(e);
		}

		bool hasActiveManager() const;
		void managerDeleted();

	protected:

		EventManager *mManager;

	};

}
}
