#pragma once

#include <base/handle.h>

#include "event.h"
#include "ievent_listener.h"
#include "event_manager.h"

#include "mouse_common.h"
#include "keyboard_common.h"

#include <string>

using namespace std;

namespace am {
namespace ui {

	class EventInterface : public virtual am::base::IManaged {
	public:

		EventInterface();
		virtual ~EventInterface();

		bool addEventListener(const char *type, IEventListener *context);
		bool addEventListener(const string &type, IEventListener *context);
		bool addEventListener(Mouse::EventType type, IEventListener *context);
		bool addEventListener(Keyboard::EventType type, IEventListener *context);
		
		bool removeEventListener(const char *type, IEventListener *context);
		bool removeEventListener(const string &type, IEventListener *context);
		bool removeEventListener(Mouse::EventType type, IEventListener *context);
		bool removeEventListener(Keyboard::EventType type, IEventListener *context);
		void removeEventListener(IEventListener *context);
		
		bool hasEventListener(const char *type);
		bool hasEventListener(const string &type);

		template <class T>
		void fireEvent(T *e)
		{
			if (mManager == nullptr)
			{
				return;
			}
			if (e->getEventTarget() == nullptr)
			{
				//e->setEventTarget(dynamic_cast<EventInterface *>(this));
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
