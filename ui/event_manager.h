#pragma once

#include"ievent_manager.h"
#include "event.h"

#include <string>
#include <map>
#include <vector>

using namespace std;

namespace am {
namespace ui {

	class EventManager : public IEventManager {
	public:
		EventManager();
		~EventManager();

		// IEventListener methods
		virtual void addEventListener(const char *type, IEventListener *content);
		virtual void addEventListener(const string &type, IEventListener *content);
		void addEventListener(MouseEventType type, IEventListener *content)
		{
			addEventListener(MouseEventTypeNames[type], content);
		}
		void addEventListener(KeyboardEventType type, IEventListener *content)
		{
			addEventListener(KeyboardEventTypeNames[type], content);
		}
		virtual void removeEventListener(const char *type, IEventListener *content);
		virtual void removeEventListener(const string &type, IEventListener *content);
		void removeEventListener(MouseEventType type, IEventListener *content)
		{
			removeEventListener(MouseEventTypeNames[type], content);
		}
		void removeEventListener(KeyboardEventType type, IEventListener *content)
		{
			removeEventListener(KeyboardEventTypeNames[type], content);
		}
		virtual bool hasEventListener(const char *type);
		virtual bool hasEventListener(const string &type);

	protected:

		ListenerList::iterator findListener(const string &type, IEventListener *context);
	};

}
}
