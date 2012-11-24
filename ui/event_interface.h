#pragma once

#include "event.h"
#include "ievent_listener.h"

#include "mouse_common.h"
#include "keyboard_common.h"

#include <string>

using namespace std;

namespace am {
namespace ui {

	class EventManager2;

	class EventInterface {
	public:

		EventInterface();
		virtual ~EventInterface();

		void addEventListener(const char *type, IEventListener *content);
		void addEventListener(const string &type, IEventListener *content);
		void addEventListener(MouseEventType type, IEventListener *content);
		void addEventListener(KeyboardEventType type, IEventListener *content);
		
		void removeEventListener(const char *type, IEventListener *content);
		void removeEventListener(const string &type, IEventListener *content);
		void removeEventListener(MouseEventType type, IEventListener *content);
		void removeEventListener(KeyboardEventType type, IEventListener *content);
		
		bool hasEventListener(const char *type);
		bool hasEventListener(const string &type);

		template <class T>
		void fireEvent(T *e)
		{
			if (mManager == NULL)
			{
				return;
			}
			mManager->fireEvent<T>(e);
		}

	protected:

		EventManager2 *mManager;

	};

}
}
